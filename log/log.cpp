// 
// Created by xuanxuan on 2022/11/20.
//

#include "log.h"

log::log() {
    m_line_count = 0;
    m_is_async = false;
    m_write_thread = nullptr;
    m_deque = nullptr;
    m_to_day = 0;
    m_fp = nullptr;
}

log::~log() {
    if(m_write_thread && m_write_thread->joinable()){
        while(!m_deque->empty()){// 不为空
            m_deque->flush();
        }
        m_deque->close();
        m_write_thread->join();
    }
    if(m_fp){
        std::lock_guard<std::mutex>locker(m_mutex);
        flush();
        fclose(m_fp);// 关闭流
    }
}

int log::get_level() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return  m_level;
}

void log::set_level(int level) {
    std::lock_guard<std::mutex>locker(m_mutex);
    m_level = level;
}

bool log::is_open() {
    //std::lock_guard<std::mutex>locker(m_mutex);
    return  m_is_open;
}

void log::init(int level = 1, const char *path, const char *suffix, int max_queue_capacity) {

    m_is_open = true;// 开启标志设置true
    m_level = level;
    if(max_queue_capacity > 0){// 合法的队列容量，开启阻塞队列，说明使用异步写入。
        m_is_async = true;//
        if(!m_deque){// 阻塞队列类还未定义
            // 注意这里的创建方法，unique_ptr的构造方法，必须使用直接初始化的方式。explicit
            // 产生一个左值？
            std::unique_ptr<block_deque<std::string>> new_deque(new block_deque<std::string>);
            m_deque = move(new_deque);// 从一个对象移动到另一个对象，转移所有权，将对应指向的内容。

            std::unique_ptr<std::thread> new_thread(new std::thread(flush_log_thread));
            m_write_thread = move(new_thread);
        }
    }else{
        m_is_async = false;
    }
    m_line_count = 0;

    time_t  timer = time(nullptr);
    struct tm* sys_time = localtime(&timer);// 获取本地时间
    struct tm t = *sys_time;

    m_path = path;
    m_suffix = suffix;
    char file_name[LOG_NAME_LEN] = {0};
    snprintf(file_name,LOG_NAME_LEN-1,"%s/%04d_%02d_%02d%s",
             m_path,
             t.tm_year + 1900,
             t.tm_mon + 1,
             t.tm_mday,
             m_suffix);

    m_to_day = t.tm_mday;

    // 这个代码块用于互斥锁
    {
        std::lock_guard<std::mutex>locker(m_mutex);
        m_buffer.retrieve_all();// 重置(初始化)缓冲区

        if(m_fp){// 如果有打开的文件流，那先关掉。
            flush();
            fclose(m_fp);
        }

        m_fp = fopen(file_name,"a");// 打开文件，追加写。
        if(m_fp == nullptr){// 打开失败
            mkdir(m_path,0777);// 判断目录是否存在，不存在就创建。
            m_fp = fopen(file_name,"a");
        }
        assert(m_fp != nullptr);// 检验
    }
}

void log::write(int level, const char *format, ...) {
    struct timeval now = {0,0};;
    gettimeofday(&now, nullptr);// 获取当前时间
    time_t t_sec = now.tv_sec;
    struct tm* sys_time = localtime(&t_sec);
    struct tm t = *sys_time;

    va_list vaList;

    // 日期变化了 或 超出最大行数
    if(m_to_day != t.tm_mday || (m_line_count && (m_line_count % MAX_LINES == 0))){
        std::unique_lock<std::mutex>locker(m_mutex);// 默认上锁
        locker.unlock();// 提前开锁，先共享一些数据

        char new_file[LOG_NAME_LEN];
        char tail[36] = {0};
        snprintf(tail,36,"%04d_%02d_%02d",
                 t.tm_year + 1900,
                 t.tm_mon + 1,
                 t.tm_mday);

        if(m_to_day != t.tm_mday){// 日期变化了
            snprintf(new_file,LOG_NAME_LEN - 72,"%s/%s%s",m_path,tail,m_suffix);
            m_to_day = t.tm_mday;
            m_line_count = 0;
        }else{
            snprintf(new_file,LOG_NAME_LEN - 72,"%s/%s-%d%s",m_path,tail,
                     (m_line_count / MAX_LINES),m_suffix);
        }

        locker.lock();// 锁上
        flush();
        fclose(m_fp);
        m_fp = fopen(new_file,"a");
        assert(m_fp != nullptr);
    }
    // 正常写入
    {
        std::unique_lock<std::mutex>locker(m_mutex);
        m_line_count++;
        int n = snprintf(m_buffer.begin_write(),128,
                         "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                         t.tm_year + 1900,
                         t.tm_mon + 1,
                         t.tm_mday,
                         t.tm_hour,
                         t.tm_min,
                         t.tm_sec,
                         now.tv_usec);

        m_buffer.has_written(n);
        append_log_level_title(level);// 添加前缀

        va_start(vaList,format);
        int m = vsnprintf(m_buffer.begin_write(),m_buffer.write_able_bytes(),
                          format,vaList);

        va_end(vaList);

        m_buffer.has_written(m);
        m_buffer.append("\n\0",2);

        /*********************/
        //std::cout<<m_buffer.retrieve_all_to_str()<<std::endl;
        /*********************/

        if(m_is_async  && m_deque && !m_deque->full()){ // 异步写入，先保存到阻塞队列中。
            m_deque->push_back(m_buffer.retrieve_all_to_str());// 将这条信息添加到阻塞队列中
        }else{// 非异步写入
            fputs(m_buffer.peek(),m_fp);// 读指针位置，即已经写入的长度。
        }
        m_buffer.retrieve_all();// 重置缓冲区，也就是说缓冲区就存一条日志信息。
    }
}

void log::append_log_level_title(int level) {
    switch(level) {
        case 0:
            m_buffer.append("[debug]: ", 9);
            break;
        case 1:
            m_buffer.append("[info] : ", 9);
            break;
        case 2:
            m_buffer.append("[warn] : ", 9);
            break;
        case 3:
            m_buffer.append("[error]: ", 9);
            break;
        default:
            m_buffer.append("[info] : ", 9);
            break;
    }
}

void log::flush() {
    if(m_is_async){// 异步
        m_deque->flush();
    }
    fflush(m_fp);// 将缓冲区中的内容写入文件。
}

void log::async_write() {
    std::string str = "";
    while(m_deque->pop(str)){// 从阻塞队列中取
        std::lock_guard<std::mutex>locker(m_mutex);
        fputs(str.c_str(),m_fp);// 写入文件
    }
}

log* log::instance() {
    static log inst;// 创建一个唯一log
    return &inst;
}

void log::flush_log_thread() {
    log::instance()->async_write();
}