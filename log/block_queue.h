// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_BLOCK_QUEUE_H
#define TINYWEBSERVER_BLOCK_QUEUE_H

#include <deque> // 双端队列
#include <assert.h>
#include <sys/time.h>
#include <condition_variable>

// 阻塞队列
template<class T>
class block_deque{
    // 初始化，默认最大长度为1000
    explicit block_deque(size_t max_capacity = 1000);

    // 析构
    ~block_deque();

    // 清空
    void clear();

    // 是否为空
    bool empty();

    // 是否满了
    bool full();

    // 关闭阻塞队列
    void close();

    // 当前队列中的元素数量
    size_t size();

    // 总容量
    size_t capacity();

    // 获取队头元素
    T front();

    // 获取队尾元素
    T back();

    // 尾插
    void push_back(const T &item);

    // 头插
    void push_front(const T& item);

    // 弹出队头元素，保存到item中
    bool pop(T &item);

    //
    bool pop(T &item,int timeout);

    //
    void flush();

private:

    // 存储容器
    std::deque<T> m_deque;

    // 容量
    size_t m_capacity;

    // 互斥锁
    std::mutex m_mutex;

    // 开启or关闭
    bool m_is_close;

    // 条件变量-消费者
    std::condition_variable m_cond_consumer;

    // 条件变量-生产者
    std::condition_variable m_cond_producer;


};

// 函数实现

template<class T>
block_deque<T>::block_deque(size_t max_capacity):
            m_capacity(max_capacity){
    assert(max_capacity > 0);// 检查是否合法
    m_is_close = false;
}

template<class T>
block_deque<T>::~block_deque() {
    close();
}

template<class T>
void block_deque<T>::close() {
    {// 声明一小块作用域
        // locker会在声明周期结束后，将m_mute解锁
        std::lock_guard<std::mutex> locker(m_mutex);
        m_deque.clear(); // 清空
        m_is_close = true;// 关闭
    }
    // 唤醒所有等待线程
    m_cond_producer.notify_all();
    m_cond_consumer.notify_all();
}

template<class T>
void block_deque<T>::flush() {

}

template<class T>
void block_deque<T>::clear(){
    std::lock_guard<std::mutex>locker(m_mutex); // 加锁
    m_deque.clear();
}

template<class T>
T block_deque<T>::front() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return m_deque.front();
}

template<class T>
T block_deque<T>::back() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return m_deque.back();
}

template<class T>
size_t block_deque<T>::size() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return m_deque.size();
}

template<class T>
size_t block_deque<T>::capacity() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return m_capacity;
}

template<class T>
void block_deque<T>::push_back(const T &item) {
    std::lock_guard<std::mutex>locker(m_mutex);
    
}



/*
 *
 *            1 0 0 0    16
 *  0 0 0 0 0   0
 *
 * */
#endif //TINYWEBSERVER_BLOCK_QUEUE_H
