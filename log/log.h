// 
// Created by xuanxuan on 2022/11/15.
//

#ifndef TINYWEBSERVER_LOG_H
#define TINYWEBSERVER_LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <stdarg.h> // 可变参数
#include <assert.h>
#include <sys/stat.h> // 文件相关信息 mkdir
#include "../block_queue/block_queue.h"
#include "../buffer/buffer.h"


class log {

public:
    void init(int level,
              const char* path = "./log",
              const char* suffix = ".log",
              int max_queue_capacity = 1024);

    // 单例模式-获取实例
    static log* instance();

    static void flush_log_thread();

    void write(int level,const char *format,...);

    void flush();

    int get_level();

    void set_level();

    vool is_open();

private:
    // 日志文件所在路径长度
    static const int LOG_PATH_LEN = 256;

    // 日志文件文件名长度
    static const int LOG_NAME_LEN = 256;

    //
    static const int MAX_LINES = 50000;

    const char* m_path;

    const char* m_suffix;

    int m_max_lines;

    int m_line_count;

    int m_to_day;

     // 缓冲区
     buffer m_buffer;

     // 日志等级水平
     int m_level;

     // 是否异步写入
     bool is_async;

     // 文件
     FILE* m_fp;


     std::unique_ptr<block_deque<std::string>> m_deque;

     std::unique_ptr<std::thread> m_write_thread;

     std::mutex m_mutex;

};

#define LOG_BASE(level,format,...) \
    do{\
        log* log = LOG::instance();\
        if(log->is_open() && log->get_level() <= level){ \
            log->write(level,format,##__VA_ARGS__);      \
            log->flush();\
        }\
    }while(0);






#endif //TINYWEBSERVER_LOG_H
