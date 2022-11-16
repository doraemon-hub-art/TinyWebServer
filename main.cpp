#include <iostream>
#include "epoller/epoller.h"

// test头文件
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <errno.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>
//#include <assert.h>
//#include <fcntl.h>
//#include <unistd.h>


int main() {

    int port = 8080;    // 端口
    int trig_mode = 0;  // 触发模式
    int timeout = 6000; // 超时时间
    bool youya = false; // 优雅退出
    int sql_port = 3306; // 数据库端口
    std::string sql_name = "root";
    std::string sql_passwd  = "123";
    std::string data_bases = "WebServer";
    int sql_conn_nums = 8; // 数据库连接池数量
    int thread_nums = 8; // 线程池线程数量
    bool log_switch = true; // 日志开关
    int log_block_queue = 1024; // 日志阻塞队列容量

    epoller* epoller_ = new epoller();

    bool is_start = true;
    while(is_start){
        int nums = epoller_->wait();
    }

    delete epoller_;
    return 0;
}
