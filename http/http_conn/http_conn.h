// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_HTTP_CONN_H
#define TINYWEBSERVER_HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <error.h>

#include "../../log/log.h"
#include "../../sql_conn_pool/sql_conn_raii.h"
#include "../../buffer/buffer.h"
#include "../http_request/http_request.h"
#include "../http_response/http_response.h"

class http_conn {
public:
    http_conn();

    ~http_conn();

    // 初始化
    void init(int sock_fd,const sockaddr_in & addr);

    // 读消息，返回读取的数量——这个返回值有待商榷
    ssize_t read(int* save_errno);

    // 错误响应
    ssize_t write(int* save_errno);

    // 断开连接
    void close_conn();

    // 获取当前连接fd
    int get_fd()const;

    // 获取当前链接过来的客户机端口
    int get_port()const;

    // 获取当前链接的客户IP
    const char* get_ip()const;

    //  获取当前链接的相关地址信息
    sockaddr_in get_addr()const;

    // 解析请求，作出响应，介于read和write之间，也就是根据读进来的数据进行分析，并准备好响应内容
    bool process();

    // 准备写入多少字节——两个IO向量中的缓冲区数据字节数之和
    int to_write_bytes();

    // 是否为长连接——要从该client socket的http请求中获取
    bool is_keep_alive()const;

public:

    // 是否为ET模式-边缘触发，只给出一次提示，所以要一次性读完
    static bool m_is_et;

    // 资源路径
    static const char* m_src_dir;

    // 连接数量
    static std::atomic<int> m_user_count;
private:

    // 连接文件描述符
    int m_fd;

    // 连接地址信息
    struct sockaddr_in m_addr;

    // 是否已经关闭
    bool m_is_close;

    // 响应内容有几块，第一块存响应头，第二块存响应内容。
    int m_iov_cnt;

    // 响应——包括响应头+响应内容
    struct iovec m_iov[2];

    // 读缓冲区
    buffer m_read_buffer;

    // 写缓冲区
    buffer m_write_buffer;

    // 请求
    http_request m_request;

    // 进行响应
    http_response m_response;
};


#endif //TINYWEBSERVER_HTTP_CONN_H
