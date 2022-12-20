// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_SERVER_H
#define TINYWEBSERVER_SERVER_H

#include <unordered_map>
#include <fcntl.h>
#include <cassert>
#include <error.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../epoller/epoller.h"
#include "../log/log.h"
#include "../heap_timer/heap_timer.h"
#include "../thread_pool/thread_pool.h"
#include "../sql_conn_pool/sql_conn_raii.h"
#include "../http/http_conn/http_conn.h"

class server {
public:
    server(
            int port,
            int trig_mode,
            int timeout_ms,
            bool opt_linger,
            int sql_port,
            const char* sql_user,
            const char* sql_pwd,
            const char* db_name,
            int conn_pool_num,
            int thread_num,
            bool open_log,
            int log_level,
            int log_que_size);

    ~server();

    // epoll开始监听
    void start();

private:

    // 创建&初始化监听socket
    bool init_socket();

    // 初始化监听模型- ET | LT
    void init_event_mode(int trig_mode);

    // 添加客户端连接
    void add_clinet(int fd,sockaddr_in addr);

    // 监听
    void deal_listen();

    // 处理写事件，往线程池里添加任务，实际调用的是下方的给客户端返回消息函数
    void deal_write(http_conn* client);

    // 处理读事件，往线程池中添加任务，实际调用的是下方的从客户端读取消息函数
    void deal_read(http_conn* client);

    // 发送错误信息
    void send_error(int fd,const char* info);

    // 往后延长指定客户端超时时间
    void extent_time(http_conn* client);

    // 关闭连接
    void close_conn(http_conn* client);

    // 从客户端读消息
    void on_read(http_conn* client);

    // 给客户端返回信息
    void on_write(http_conn* client);

    // 作出响应，准备好服务器要响应的内容
    void on_process(http_conn* client);

private:

    // 最大文件描述符
    static const int MAX_FD = 65535;

    // 设置非阻塞IO
    static int set_fd_non_block(int fd);

private:

    // 服务器端口
    int m_port;

    // 是否优雅关闭，优雅关闭——直到所剩数据发送完毕或超时后才关闭
    bool m_open_linger;

    // 超时时间
    int m_timeout_ms;

    // 是否关闭
    bool m_is_close;

    // 监听socket文件描述符
    int m_listen_fd;

    // 资源路径
    char* m_src_dir;

    // 监听socket监听事件属性
    uint32_t m_listen_event;

    // 连接socket监听事件属性
    uint32_t m_conn_event;

    // 超时堆
    std::unique_ptr<heap_timer> m_timer;

    // 线程池
    std::unique_ptr<thread_pool> m_thread_pool;

    // epoll
    std::unique_ptr<epoller> m_epoller;

    // 连接socket fd与其对应的http_conn连接信息
    std::unordered_map<int,http_conn>m_users;
};


#endif //TINYWEBSERVER_SERVER_H
