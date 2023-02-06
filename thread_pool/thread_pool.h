// 
// Created by xuanxuan on 2022/11/28.
//

#ifndef TINYWEBSERVER_THREAD_POOL_H
#define TINYWEBSERVER_THREAD_POOL_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
#include <cassert>

class thread_pool{
public:
    explicit thread_pool(size_t thread_count = 8):m_pool(std::make_shared<pool>()){
        assert(thread_count > 0);// 验证线程数量是否合法
        for(size_t i = 0; i < thread_count;i++){
            std::thread([Pool = m_pool]{// lambda-线程执行函数,创建线程，线程任务
                std::unique_lock<std::mutex>locker(Pool->m_mutex);
                while(1){//尝试取任务
                    if(!Pool->m_tasks.empty()){// 有任务
                        auto task = std::move(Pool->m_tasks.front());
                        Pool->m_tasks.pop();
                        locker.unlock();
                        task();// 执行
                        locker.lock();
                    }else if(Pool->m_is_closed){// 关闭了
                        break;
                    }else{// 没有任务啦
                        Pool->m_cond.wait(locker);// 使线程等待
                    }
                }
            }).detach();// 将子线程和从主线程中分离
        }
    }

    thread_pool() = default;

    thread_pool(thread_pool &&) = default;

    ~thread_pool(){
        if(static_cast<bool>(m_pool)){
            {
                std::lock_guard<std::mutex>locker(m_pool->m_mutex);
                m_pool->m_is_closed = true;// 关闭标识
            }
            m_pool->m_cond.notify_all();// 唤醒所有等待线程
        }
    }

    // 添加任务
    template<class T>
    void add_task(T&& task){
        {
            std::lock_guard<std::mutex>locker(m_pool->m_mutex);// 锁

            // forward完美转发，防止多余拷贝
            m_pool->m_tasks.emplace(std::forward<T>(task));// 放入任务队列
        }
        m_pool->m_cond.notify_one();// 唤醒一个等待线程
    }

private:
    // -一个池子结构
    struct pool{
        std::mutex m_mutex;
        std::condition_variable m_cond;
        bool m_is_closed;// 关闭标志
        std::queue<std::function<void()>>m_tasks;// 任务队列
    };

    std::shared_ptr<pool>m_pool;// 池
};


#endif //TINYWEBSERVER_THREAD_POOL_H