// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_SERVER_H
#define TINYWEBSERVER_SERVER_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include "../epoller/epoller.h"

class server {
public:
    server(int port);

private:
    int m_server_port; // 运行端口
    int m_timeout; //
    bool m_is_close; // 是否关闭

    int m_listen_fd; // 监听socket

    std::unique_ptr<epoller> m_epoll_fd;

};


#endif //TINYWEBSERVER_SERVER_H
