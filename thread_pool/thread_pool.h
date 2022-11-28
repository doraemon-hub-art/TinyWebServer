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
        assert(thread_count > 0);
        for(size_t i = 0; i < thread_count;i++){
            std::thread([pool = m_pool]{
                std::unique_lock<std::mutex>locker(pool->m_mutex);
                while(1){
                    if(!pool->m_tasks.empty()){// 有任务
                        auto task = std::move(pool->m_tasks.front());
                        pool->m_tasks.pop();
                        locker.unlock();
                        task();// 执行
                        locker.lock();
                    }else if(pool->m_is_closed){// 关闭了
                        break;
                    }else{
                        pool->m_cond.wait(locker);// 使线程等待
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
            m_pool->m_cond.notify_all();
        }
    }

    // 添加任务
    template<class T>
    void add_task(T&& task){
        {
            std::lock_guard<std::mutex>locker(m_pool->m_mutex);

            // forward完美转发，防止多余拷贝
            m_pool->m_tasks.emplace(std::forward<T>(task));// 放入任务队列
        }
    }

private:
    // 池子内容封装
    struct pool{
        std::mutex m_mutex;
        std::condition_variable m_cond;
        bool m_is_closed;// 关闭标志
        std::queue<std::function<void()>>m_tasks;// 任务队列
    };

    std::shared_ptr<pool>m_pool;// 池
};


#endif //TINYWEBSERVER_THREAD_POOL_H