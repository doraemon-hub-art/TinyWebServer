// 
// Created by xuanxuan on 2022/11/16.
//

#include "server.h"

server::server(int port, int trig_mode, int timeout_ms, bool opt_linger, int sql_port, const char *sql_user,
               const char *sql_pwd, const char *db_name, int conn_pool_num, int thread_num, bool open_log,
               int log_level, int log_que_size):m_port(port),m_open_linger(opt_linger),m_timeout_ms(timeout_ms),m_is_close(
        false),m_timer(new heap_timer()),m_thread_pool(new thread_pool(thread_num)),m_epoller(new epoller()){

    m_src_dir = getcwd(nullptr,256);// 获取当前程序的工作路径
    //std::cout<<m_src_dir<<std::endl;
    assert(m_src_dir);
    strncat(m_src_dir,"/resources/",16);// 追加/resources/
    http_conn::m_user_count = 0;// 当前用户初始化为0
    http_conn::m_src_dir = m_src_dir;
    sql_conn_pool::instance()->init("localhost",sql_port,sql_user,sql_pwd,db_name,conn_pool_num);

    init_event_mode(trig_mode);
    if(!init_socket()){// 初始化监听socket失败
        m_is_close = true;
    }

    if(open_log){// 打开日志
        log::instance()->init(log_level,"./log/logs",".log",log_que_size);
        if(m_is_close){
            LOG_ERROR("========== Server init error!==========");
        }else{
            LOG_INFO("========== Server init ==========");
            LOG_INFO("Port:%d, OpenLinger: %s", m_port, opt_linger ? "true":"false");
            LOG_INFO("Listen Mode: %s, OpenConn Mode: %s",
                     (m_listen_event & EPOLLET ? "ET": "LT"),
                     (m_conn_event & EPOLLET ? "ET": "LT"));
            LOG_INFO("LogSys level: %d", log_level);
            LOG_INFO("srcDir: %s", http_conn::m_src_dir);
            LOG_INFO("SqlConnPool num: %d, ThreadPool num: %d", conn_pool_num, thread_num);
        }
    }
}

server::~server() {
    close(m_listen_fd);
    m_is_close = true;
    free(m_src_dir);
    sql_conn_pool::instance()->close_pool();// 关闭连接池
}

void server::init_event_mode(int trig_mode) {
    m_listen_event = EPOLLRDHUP;
    m_conn_event = EPOLLONESHOT | EPOLLRDHUP;

    switch (trig_mode) {
    case 0:
        break;
    case 1:
        m_conn_event |= EPOLLET;
        break;
    case 2:
        m_listen_event |= EPOLLET;
        break;
    case 3:
        m_listen_event |= EPOLLET;
        m_conn_event |= EPOLLET;
        break;
    default:
        m_listen_event |= EPOLLET;
        m_conn_event |= EPOLLET;
        break;
    }
    http_conn::m_is_et = (m_conn_event & EPOLLET);
}

void server::start() {
    int time_ms = -1;// 设置epoll是否阻塞
    if(!m_is_close){// 服务器处于开启状态
        LOG_INFO("========== Server start ==========");
    }
    while(!m_is_close){
        if(m_timeout_ms > 0){
            time_ms = m_timer->get_next_tick();// 距离下一个要超时的定时器还有多长时间
        }
        int event_cnt = m_epoller->wait(time_ms);//这样根据时间等待更加智能

        for(int i = 0; i < event_cnt;i++){// 处理响应的时间
            int fd = m_epoller->get_event_fd(i);
            uint32_t event = m_epoller->get_event(i);
            if(fd == m_listen_fd){// 监听socket上的时间，
                deal_listen();
            }else if(event & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){// 错误事件
                assert(m_users.count(fd) > 0);// 存在这个连接
                close_conn(&m_users[fd]);
            }else if(event & EPOLLIN){// 读事件
                assert(m_users.count(fd) > 0);
                deal_read(&m_users[fd]);
            }else if(event & EPOLLOUT){// 写事件
                assert(m_users.count(fd) > 0);
                deal_write(&m_users[fd]);
            }else{
                LOG_ERROR("Unexpected event.");// 未知事件
            }
        }
    }
}

void server::send_error(int fd, const char *info) {
    assert(fd > 0);
    int ret = send(fd,info, strlen(info),0);
    if(ret < 0){
        LOG_WARN("send error to client[%d] error!",fd);
    }
    close(fd);
}

void server::close_conn(http_conn *client) {
    assert(client);
    LOG_INFO("Client[%d] quit!",client->get_fd());
    m_epoller->del_fd(client->get_fd());
    client->close_conn();
}

void server::add_clinet(int fd, sockaddr_in addr) {
    assert(fd > 0);
    m_users[fd].init(fd,addr);
    if(m_timeout_ms > 0){
        // 将当前连接放到定时器堆中，其中具体的实现是获取当前时间加上超时时间
        m_timer->add(fd,m_timeout_ms,std::bind(&server::close_conn,this,&m_users[fd]));
    }
    m_epoller->add_fd(fd,EPOLLIN | m_conn_event);
    set_fd_non_block(fd);// 设置非阻塞
    LOG_INFO("Client[%d] in!",m_users[fd].get_fd());
}

void server::deal_listen() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    do{
        int fd = accept(m_listen_fd,(struct sockaddr*)&addr,&len);
        if(fd <= 0){
            return ;
        }else if(http_conn::m_user_count >= MAX_FD){// 连接数满啦
            send_error(fd,"Server busy!");
            LOG_WARN("Clients is full!");
            return ;
        }
        add_clinet(fd,addr);
    }while(m_listen_event & EPOLLET);// 监听socket是否为ET模式
    // 监听SOCKET的ET模式，应该是如果本次服务器没处理对应客户端的连接请求，那就需要等待下次了，并不是这辈子连不上了。
    // 所以如果是ET模式的监听SOCKET就需要将一下子发起请求的所有客户端都通过accept
}

void server::deal_read(http_conn *client) {
    assert(client);
    extent_time(client);
    m_thread_pool->add_task(std::bind(&server::on_read,this,client));
}

void server::deal_write(http_conn *client) {
    assert(client);
    extent_time(client);
    m_thread_pool->add_task(std::bind(&server::on_write,this,client));
}

void server::extent_time(http_conn *client) {
    assert(client);
    if(m_timeout_ms > 0){
        m_timer->adjust(client->get_fd(),m_timeout_ms);
    }
}

void server::on_read(http_conn *client) {
    assert(client);
    int ret = -1;
    int read_errno = 0;
    ret = client->read(&read_errno);
    if(ret <= 0 && read_errno != EAGAIN){
        close_conn(client);
        return ;
    }
    on_process(client);
}

void server::on_process(http_conn *client) {
    if(client->process()){// 准备好了，将对应的监听事件改为写事件
        m_epoller->mod_fd(client->get_fd(),m_conn_event | EPOLLOUT);
    }else{// 还没准备好，依然监听读事件
        m_epoller->mod_fd(client->get_fd(),m_conn_event | EPOLLIN);
    }
}

void server::on_write(http_conn *client) {
    assert(client);
    int ret = -1;
    int write_errno = 0;
    ret = client->write(&write_errno);
    if(client->to_write_bytes() == 0){// 传输完成
        if(client->is_keep_alive()){
            on_process(client);
            return ;
        }
    }else if(ret < 0){
        if(write_errno == EAGAIN){
            // 继续传输
            m_epoller->mod_fd(client->get_fd(),m_conn_event | EPOLLOUT);
            return ;
        }
    }
    close_conn(client);// 出现错误，关闭连接
}

bool server::init_socket() {
    int ret;
    struct sockaddr_in addr;
    if(m_port > 65535 || m_port < 1024){// 端口号限制
        LOG_ERROR("Port[%d]: error",m_port);
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(m_port);
    struct linger opt_linger = {0};
    if(m_open_linger){// 优雅关闭
        opt_linger.l_onoff = 1;
        opt_linger.l_linger = 1;
    }

    m_listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listen_fd < 0){
        LOG_ERROR("Create socket error!",m_port);
        return false;
    }

    ret = setsockopt(m_listen_fd,SOL_SOCKET,SO_LINGER,&opt_linger,sizeof(opt_linger));
    if(ret < 0){
        close(m_listen_fd);
        LOG_ERROR("init linger error!",m_port);
        return false;
    }

    int opt_val = 1;
    // 设置端口复用
    ret = setsockopt(m_listen_fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt_val,sizeof(opt_val));
    if(ret == -1){
        LOG_ERROR("Set socket setsockopt error!");
        close(m_listen_fd);
        return false;
    }

    ret = bind(m_listen_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0){
        LOG_ERROR("Bind port:%d error",m_port);
        close(m_listen_fd);
        return false;
    }

    ret = listen(m_listen_fd,6);
    if(ret < 0){
        LOG_ERROR("Listen port[%d]: error!");
        close(m_listen_fd);
        return false;
    }

    ret = m_epoller->add_fd(m_listen_fd,m_listen_event | EPOLLIN);
    if(ret == 0){
        LOG_ERROR("Add listen error!");
        close(m_listen_fd);
        return false;
    }

    set_fd_non_block(m_listen_fd);// 设置非阻塞
    LOG_INFO("Server port[%d]: .",m_port);
    return true;
}

int server::set_fd_non_block(int fd) {
    assert(fd > 0);
    int old_opt = fcntl(fd,F_GETFL);// 拿到之前对该fd的设置属性
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_opt);
    return new_opt;// 返回了设置非阻塞后，fd的属性，但是实际上这个返回值没用到。
}