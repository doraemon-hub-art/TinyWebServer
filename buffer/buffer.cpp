// 
// Created by xuanxuan on 2022/11/19.
//

#include "buffer.h"

buffer::buffer(int init_buffer_size):
    m_buffer(init_buffer_size),
    m_read_pos(0),
    m_write_pos(0){
}

size_t buffer::read_able_bytes() const {
    return m_write_pos - m_read_pos;
}

size_t buffer::write_able_bytes() const {
    return  m_buffer.size() - m_write_pos;
}

size_t buffer::prepend_able_bytes() const {
    return m_read_pos;
}

const char* buffer::peek() const {
    return begin_ptr()+m_read_pos;
}

void buffer::retrieve(size_t len){
    assert(len <= read_able_bytes());// 位置是否合法，小于已经写入进缓冲区中但是还没读到的数量，即m_write_pos-m_read_pos
    m_read_pos += len;
}

void buffer::retrieve_until(const char *end) {
    assert(peek() <= end);// 位置是否合法，end在该要读取位置的后面
    retrieve(end - peek());// 两个指针做减法为其中的元素个数，当指向的是同一个数组时。
    // 调用，如果合法，end指向的地方，在[m_read_pos,n_write_pos]范围内，m_read_pos向后偏移
}

void buffer::retrieve_all() {
    bzero(&m_buffer[0],m_buffer.size());
    m_read_pos = 0;
    m_write_pos = 0;
}

std::string buffer::retrieve_all_to_str() {
    std::string str(peek(),read_able_bytes());
    retrieve_all();// 清空
    return str;
}

const char* buffer::begin_write_const() const {
    return begin_ptr() + m_write_pos;
}

char* buffer::begin_write() {
    return begin_ptr() + m_write_pos;
}

void buffer::has_written(size_t len) {
    m_write_pos += len;
}

void buffer::append(const char *str, size_t len) {
    assert(str);
    ensure_write_able(len);
    std::copy(str,str+len,begin_write());
    has_written(len);
}

void buffer::append(const void *data, size_t len) {
    assert(data); // 是否为空指针
    append(static_cast<const char*>(data),len);
}


void buffer::append(const std::string &str) {
    append(str.data(),str.length());
}

void buffer::append(const buffer &buff) {
    // 从另个一个缓冲区读过来
    // append(该读到哪了的指针，还有多少内容没读的长度)
    append(buff.peek(),buff.read_able_bytes());
}

void buffer::make_space(size_t len) {
    // 如果，缓冲区的剩余空间+已经读取的大小，不够存储了。
    if(write_able_bytes() + prepend_able_bytes() < len){
        // 更新缓冲区大小为: 原来的数据+要加入的数据+1
        m_buffer.resize(m_write_pos + len + 1);// 重新调整缓冲区大小
    }else{// 空间还是够的
        size_t read_able = read_able_bytes(); // 还没读取到的大小
        // 将还没读取过数据的拷贝到当前容器开头，将已经读取过的覆盖掉。
        std::copy(begin_ptr() + m_read_pos,begin_ptr() + m_write_pos,begin_ptr());// 内容拷贝
        // 更新偏移
        m_read_pos = 0;
        m_write_pos = m_read_pos + read_able;
        assert(read_able == read_able_bytes());// 验证
    }
}

void buffer::ensure_write_able(size_t len) {
    if(write_able_bytes() < len){// 空间不够了
        make_space(len);// 扩充
    }
    assert(write_able_bytes() >=len);// 验证现在够了吗
}


char* buffer::begin_ptr() {
    return &*m_buffer.begin();
}

const char* buffer::begin_ptr() const {
    return &*m_buffer.begin();
}

ssize_t buffer::read_fd(int fd, int* save_errno) {
    char buff[65535];
    struct iovec iov[2];
    // m_buffer中剩余可写入的空间
    const size_t writable = write_able_bytes();

    // 分散读，保证数据全部读完
    iov[0].iov_base = begin_ptr() + m_write_pos;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    // 先往m_buffer中读，如果空间不够，则读到buff中。
    const ssize_t len = readv(fd,iov,2);

    if(len < 0){// 没读到
        *save_errno = errno;
    }else if(static_cast<size_t>(len) <= writable){//m_buffer中的空间就已经够了,仅仅更新读取偏移。
        m_write_pos += len;
    }else{// m_buffer中的空间不够，说明buff中存有读取到的内容，则将该内容追加到m_buffer中.
        m_write_pos = m_buffer.size();
        append(buff,len-writable);
    }
    return len;
}

ssize_t buffer::write_fd(int fd, int *save_errno) {
    size_t readSize = read_able_bytes();// m_buffer中还剩都少数据没读
    ssize_t len = write(fd,peek(),readSize);
    if(len < 0){
        *save_errno = errno;
        return len;
    }
    m_read_pos += len;// 更新读便宜
    return len;
}