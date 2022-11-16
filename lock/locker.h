// 
// Created by xuanxuan on 2022/11/16.
//

#ifndef TINYWEBSERVER_LOCKER_H
#define TINYWEBSERVER_LOCKER_H

#include <pthread.h>
#include <semaphore.h>
#include <exception>

// 信号量类封装
class sem{
public:
    // 默认设定信号量初始值为0
    sem();

    // 手动设置信号量初始值
    sem(int num);

    // 析构
    ~sem();

    // p操作-申请
    bool wait();

    // v操作-释放
    bool post();

private:
    sem_t m_sem;
};

// 互斥锁类封装
class locker {
public:
    locker();

    ~locker();

    // 加锁
    bool lock();

    // 开锁
    bool unlock();

    // 得到当前锁
    pthread_mutex_t * get();

private:
    pthread_mutex_t m_mutex;
};

// 条件变量类封装
class cond{
public:
    cond();

    ~cond();

    // 等待条件变量被唤醒
    bool wait(pthread_mutex_t *m_mutex);

    // 等待条件变量超时被唤醒
    bool timewait(pthread_mutex_t *m_mutex,struct timespec t);

    // 唤醒一个等待线程
    bool signal();

    // 唤醒等所有等待线程
    bool broadcast();

private:
    pthread_cond_t m_cond;
};

#endif //TINYWEBSERVER_LOCKER_H
