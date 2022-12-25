// 
// Created by xuanxuan on 2022/11/16.
//

#include "http_conn.h"

// 全局变量默认初始化为空
const char* http_conn::m_src_dir;

std::atomic<int> http_conn::m_user_count;

bool http_conn::m_is_et;

http_conn::http_conn() {
    m_fd = -1;
    m_addr = {0};
    m_is_close = true;
}

http_conn::~http_conn() {
    close_conn();
}

void http_conn::init(int sock_fd, const sockaddr_in &addr) {
    assert( sock_fd > 0);

    m_user_count++;
    m_addr = addr;
    m_fd = sock_fd;
    m_write_buffer.retrieve_all();// 调用重置缓冲区
    m_read_buffer.retrieve_all();
    m_is_close = false;

    LOG_INFO("Client[%d](%s:%d) in,current userCount:%d",m_fd,get_ip(),get_port(),(int)m_user_count);
}

void http_conn::close_conn() {
    m_response.unmap_file();// 解除文件映射
    if(!m_is_close){// 是否已经关闭
        m_is_close = true;
        m_user_count--;
        close(m_fd);
        LOG_INFO("Client[%d](%s:%d) quit,current userCount:%d",m_fd,get_ip(),get_port(),(int)m_user_count);
    }
}

int http_conn::get_fd() const {
    return m_fd;
}

struct sockaddr_in http_conn::get_addr() const {
    return m_addr;
}

const char* http_conn::get_ip() const {
    return inet_ntoa(m_addr.sin_addr);
}

int http_conn::get_port() const {
    return m_addr.sin_port;
}

ssize_t http_conn::read(int *save_errno) {
    ssize_t len = -1;
    // 感觉这里面应该是+=，因为可能要读取很多次，先做个标记，暂时先不考虑。
    do{
        len = m_read_buffer.read_fd(m_fd,save_errno);
        if(len <= 0){
            break;
        }
    }while(m_is_et);//  为真，说明为边缘触发，要一次性全部读完，否则条件触发，只读一次。
    return len;
}

ssize_t http_conn::write(int *save_errno) {
    ssize_t len = -1;
    do{
        len = writev(m_fd,m_iov,m_iov_cnt);// 使用IO向量指向的缓冲区一次往里面写
        if(len <= 0){
            *save_errno = errno;
            break;
        }
        if(m_iov[0].iov_len + m_iov[1].iov_len == 0) {// 空了，传输完成了
            break;
        }else if(static_cast<size_t>(len) > m_iov[0].iov_len){// m_iov[0]中的内容已经读完了，从m_iov[1]中继续读

            m_iov[1].iov_base = (uint8_t*)m_iov[1].iov_base + (len - m_iov[0].iov_len);// 更新读取位置

            m_iov[1].iov_len -= (len-m_iov[0].iov_len);

            if(m_iov[0].iov_len){// 非空
                m_write_buffer.retrieve_all();
                m_iov[0].iov_len = 0;
            }
        }else{// m_iov[1]中的内容还未读完
            m_iov[0].iov_base = (uint8_t*)m_iov[0].iov_base + len;
            m_iov[0].iov_len -= len;
            m_write_buffer.retrieve(len);
        }
    } while (m_is_et || to_write_bytes() > 10240);// 边缘触发或者准备写入的内容太多了，就要一直写
    return len;
}

bool http_conn::process() {
    m_request.init();

    if(m_read_buffer.read_able_bytes() <= 0){//
        return false;
    }else if(m_request.parse(m_read_buffer)){// 解析请求
        LOG_DEBUG("%s",m_request.path().c_str());
        m_response.init(m_src_dir,m_request.path(),m_request.is_keep_alive(),200);
    }else{
        m_response.init(m_src_dir,m_request.path(), false,400);
    }

    // 作出响应
    m_response.make_response(m_write_buffer);
    // 响应头-const_cast将对应内容转成可修改的变量
    m_iov[0].iov_base = const_cast<char*>(m_write_buffer.peek());
    m_iov[0].iov_len = m_write_buffer.read_able_bytes();
    m_iov_cnt = 1;

    // 文件-存在并且内容不为空
    if(m_response.file_len() > 0 && m_response.file()){
        m_iov[1].iov_base = m_response.file();
        m_iov[1].iov_len = m_response.file_len();
        m_iov_cnt = 2;
    }
    LOG_DEBUG("file size:%d,%d to %d",m_response.file_len(),m_iov_cnt,to_write_bytes());
    return true;
}

int http_conn::to_write_bytes() {
    return m_iov[0].iov_len + m_iov[1].iov_len;
}

bool http_conn::is_keep_alive() const {
    return m_request.is_keep_alive();
}







