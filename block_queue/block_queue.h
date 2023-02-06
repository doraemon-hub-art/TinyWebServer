// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_BLOCK_QUEUE_H
#define TINYWEBSERVER_BLOCK_QUEUE_H

#include <deque> // 双端队列
#include <assert.h>
#include <sys/time.h>
#include <condition_variable>
#include <mutex>

// 阻塞队列
template<class T>
class block_deque{

public:
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

    // 弹出队头元素，增加等待时间选项。
    bool pop(T &item,int timeout);

    // 唤醒一个消费者等待线程
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
    // 唤醒所有等待线程，消费者&生产者
    m_cond_producer.notify_all();
    m_cond_consumer.notify_all();
}

template<class T>
void block_deque<T>::flush() {
    m_cond_consumer.notify_one();// 唤醒一个消费者线程
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
    std::unique_lock<std::mutex>locker(m_mutex);
    while(m_deque.size() >= m_capacity){// 队列中的数量大于允许的最大容量
        // 先解锁该互斥锁，然后阻塞当前执行线程，之后等待唤醒。
        m_cond_producer.wait(locker);
    }
    m_deque.push_back(item);
    m_cond_consumer.notify_one();// 唤醒一个消费者等待线程
}

template<class T>
void block_deque<T>::push_front(const T &item) {
    // 同上，只不过将尾插改成了前插。
    std::unique_lock<std::mutex>locker(m_mutex);
    while(m_deque.size() >= m_capacity){
        m_cond_producer.wait(locker);
    }
    m_deque.push_front(item);
    m_cond_consumer.notify_one();// 唤醒一个消费者线程
}

template<class T>
bool block_deque<T>::empty() {
    std::lock_guard<std::mutex>locker(m_mutex);
    if(m_deque.empty()){
        return true;
    }else{
        return false;
    }
}

template<class T>
bool block_deque<T>::full() {
    std::lock_guard<std::mutex>locker(m_mutex);
    if(m_deque.size() >= m_capacity){
        return true;
    }else{
        return false;
    }
}

template<class T>
bool block_deque<T>::pop(T &item) {
    std::unique_lock<std::mutex>locker(m_mutex);
    while(m_deque.empty()){// 空的,空队列无法pop
        m_cond_consumer.wait(locker);
        if(m_is_close){
            return false;
        }
    }
    item = m_deque.front();
    m_deque.pop_front();
    m_cond_producer.notify_one();// 唤醒一个生产者等待线程
    return true;
}

template<class T>
bool block_deque<T>::pop(T &item, int timeout) {
    std::unique_lock<std::mutex>locker(m_mutex);
    while(m_deque.empty()){// 空的，空队列，无法pop
        // 等待timeout
        if(m_cond_consumer.wait_for(locker,std::chrono::seconds(timeout)) == std::cv_status::timeout){
            return false;
        }
        if(m_is_close){
            return false;
        }
    }
    item = m_deque.front();
    m_deque.pop_front();
    m_cond_producer.notify_one();// 唤醒一个生产者等待线程
    return  true;
}

#endif //TINYWEBSERVER_BLOCK_QUEUE_H
