// 
// Created by xuanxuan on 2022/11/19.
//

#ifndef TINYWEBSERVER_BUFFER_H
#define TINYWEBSERVER_BUFFER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>

class buffer {

public:

    // 构造函数-初始化缓冲区大小
    explicit buffer(int init_buffer_size = 1024);

    // 析构-使用默认析构
    ~buffer() = default;

    // 可写入大小
    size_t write_able_bytes()const;

    // 可读取大小，即还未读取的大小
    size_t read_able_bytes()const;

    // 已经读了多少，获取当前读取偏移，m_read_pos
    size_t prepend_able_bytes()const;

    // 获取指向该读取元素位置的指针
    const char* peek()const;

    // 确保剩余缓冲区剩余空间够写len个字节的
    void ensure_write_able(size_t len);

    // 更新写偏移
    void has_written(size_t len);

    // 定位读指针到某一位置
    void retrieve(size_t len);

    // 定位读指针到某一位置，在内部调用retrieve
    void retrieve_until(const char* end);

    // 重置缓冲区
    void retrieve_all();

    // 将缓冲区中的内容构造为string
    std::string retrieve_all_to_str();

    // 获取指向缓冲区中可写入位置的指针
    char* begin_write();

    // 获取指向缓冲区中可写入位置的指针 const
    const char* begin_write_const()const;


    // 几个append的重载函数，根据不同传参调用。

    // 将str内容写入到缓冲区中
    void append(const char* str,size_t len);
    void append(const std::string& str);
    void append(const void* data,size_t len);
    void append(const buffer& buff);

    // 从fd中读取数据
    ssize_t read_fd(int fd,int* save_errno);

    // 向fd写入数据
    ssize_t write_fd(int fd,int* save_errno);

private:

    // 拿到指向缓冲区开头的指针
    char* begin_ptr();

    // 拿到指向缓冲区开头的指针 const
    const char* begin_ptr()const;

    // 给缓冲区扩充len个字节的空间
    void make_space(size_t len);

private:

    // 存储容器
    std::vector<char>m_buffer;

    // 读到哪了-读偏移量
    std::atomic<std::size_t>m_read_pos;

    // 写到哪了-写偏移量
    std::atomic<std::size_t>m_write_pos;
};


#endif //TINYWEBSERVER_BUFFER_H
