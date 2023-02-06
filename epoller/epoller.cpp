// 
// Created by xuanxuan on 2022/11/16.
//

#include "epoller.h"

epoller::epoller(int maxEvent):
            m_epoll_fd(epoll_create(512)),
            m_events(maxEvent){
    // 验证参数是否正确
    assert(m_epoll_fd >= 0 && m_events.size() > 0);
}

epoller::~epoller() {
    close(m_epoll_fd);
}

bool epoller::add_fd(int fd, uint32_t event) {
    if(fd < 0){// 非法文件描述符
        return false;
    }
    epoll_event ev = {0};// 初始化为0
    ev.data.fd = fd;
    ev.events = event;
    if(epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,fd,&ev) == 0){
        return true;
    }else{
        return false;
    }
}

bool epoller::mod_fd(int fd, uint32_t event) {
    if(fd < 0){
        return false;
    }
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = event;
    if(epoll_ctl(m_epoll_fd,EPOLL_CTL_MOD,fd,&ev) == 0){
        return true;
    }else{
        return false;
    }
}

bool epoller::del_fd(int fd) {
    if(fd < 0){
        return false;
    }
    epoll_event ev = {0};
    if(epoll_ctl(m_epoll_fd,EPOLL_CTL_DEL,fd,&ev) == 0){
        return  true;
    }else{
        return false;
    }
}

int epoller::wait(int timeout) {
    // 注意: vector创建的数组，不能像普通数组那样传首地址进去。这里要传第一个元素地址。
//    if(epoll_wait(m_epoll_fd,&m_events[0],static_cast<int>(m_events.size()),timeout)){
//        return true;
//    }else{
//        return false;
//    }
    return epoll_wait(m_epoll_fd,&m_events[0],static_cast<int>(m_events.size()),timeout);
}

int epoller::get_event_fd(size_t index) const {
    assert(index >= 0 && index < m_events.size());// 检查下标是否合法
    return m_events[index].data.fd;
}

uint32_t epoller::get_event(size_t index) const {
    assert(index >=0 && index < m_events.size());// 检查下标是否合法
    return  m_events[index].events;
}




