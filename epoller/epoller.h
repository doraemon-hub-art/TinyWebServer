// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_EPOLLER_H
#define TINYWEBSERVER_EPOLLER_H

#include <sys/epoll.h> // epoll
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <vector> // vector
#include <assert.h> // assert()
#include <errno.h> // errno

class epoller {
public:
    // 构造，maxEvent表明最大事件表大小。
    explicit epoller(int maxEvent = 1024);

    // 析构
    ~epoller();

    // 将描述符添加到epoll的监听事件中
    bool add_fd(int fd,uint32_t event);

    // 修改文件描述符的事件
    bool mod_fd(int fd,uint32_t event);

    // 从epoll监听事件中删除该文件描述符
    bool del_fd(int fd);

    // epoll开始监听，等待时有fd响应,默认-1表示无限期阻塞，直到有事件发生。
    // 返回有事件发生的文件描述符个数
    int wait(int timeout = -1);

    // 获取保存在事件数组中的指定下标的事件的文件描述符
    int get_event_fd(size_t index)const;

    // 获取指定下标的事件
    uint32_t get_event(size_t index)const;

private:
    int m_epoll_fd;// epoll fd

    // 保存产生响应的fd和对应事件
    std::vector<struct epoll_event> m_events;

};


#endif //TINYWEBSERVER_EPOLLER_H
