// 
// Created by xuanxuan on 2022/11/20.
//

#ifndef TINYWEBSERVER_LOG_H
#define TINYWEBSERVER_LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h> // 可变参数
#include <assert.h>
#include <sys/stat.h>   // 文件相关
#include "../block_queue/block_queue.h"
#include "../buffer/buffer.h"

class log {
public:

    // 初始化
    void init(int level,
              const char* path = "./log/logs",
              const char* suffix = ".log",
              int max_queue_capacity = 1024);

    // 单例模式，获取唯一实例，懒汉模式创建时构造。
    static log* instance();

    // 调用异步写入
    static void flush_log_thread();

    // 写入文件——内部区分-异步/同步，异步写入阻塞队列，同步直接写入文件
    void write(int level,const char* format,...);

    // 将缓冲区内容写入文件
    void flush();

    // 获取当前日志等级
    int get_level();

    // 设置日志等级
    void set_level(int level);

    // 日志是否打开
    bool is_open();

private:

    // 单例模式，构造函数私有化。
    log();

    virtual  ~log();

    // 日志信息添加前缀
    void append_log_level_title(int level);

    // 异步写入
    void async_write();

private:
    // C++11还是14以后，成员变量可以直接定义赋值初始化。

    // 日志文件路径长度
    static const int LOG_PATH_LEN = 256;

    // 日志文件名字长度
    static const int LOG_NAME_LEN = 256;

    // 最大行数
    static const int MAX_LINES =  50000;

    // 路径
    const char* m_path;

    // 后缀
    const char* m_suffix;

    // 最大行数
    int m_max_lines;

    // 当前行数计数器
    int m_line_count;

    // 当月中的第几天
    int m_to_day;

    // 是否打开
    bool m_is_open;

    // 缓冲区
    buffer m_buffer;

    // 日志等级
    int m_level;

    // 异步写入开关
    bool m_is_async;

    // 指向目标写入文件的文件指针
    FILE * m_fp;

    // 阻塞队列
    std::unique_ptr<block_deque<std::string>> m_deque;

    // 线程指针-没用到这b东西
    std::unique_ptr<std::thread> m_write_thread;

    // 互斥锁
    std::mutex m_mutex;

};

#define  LOG_BASE(level,format,...) \
    do{\
       log *Log = log::instance();\
       if(Log->is_open() && Log->get_level() <= level){ \
          Log->write(level,format,##__VA_ARGS__);\
          Log->flush();\
       }\
    }while(0);

#define LOG_DEBUG(format,...)do{LOG_BASE(0,format,##__VA_ARGS__)}while(0);
#define LOG_INFO(format,...)do{LOG_BASE(1,format,##__VA_ARGS__)}while(0);
#define LOG_WARN(format,...)do{LOG_BASE(2,format,##__VA_ARGS__)}while(0);
#define LOG_ERROR(format,...)do{LOG_BASE(3,format,##__VA_ARGS__)}while(0);

#endif //TINYWEBSERVER_LOG_H
