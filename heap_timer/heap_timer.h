// 
// Created by xuanxuan on 2022/11/20.
//

#ifndef TINYWEBSERVER_HEAP_TIMER_H
#define TINYWEBSERVER_HEAP_TIMER_H


#include <queue>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h>
#include <functional>
#include <assert.h>
#include <chrono> // 时钟类
#include "../log/log.h"

// fd为指定的client socket fd，在m_ref中保存着其在堆数组中的下标。

// 回调函数
typedef std::function<void()> TIME_OUT_CALl_BACK;

// 高精度时钟 now()方法获取当前时间
typedef std::chrono::high_resolution_clock CLOCK;

// 毫秒
typedef std::chrono::milliseconds MS;

// 指向时间中的一个点
typedef CLOCK::time_point TIME_STAMP;

// 堆中结点-定时器
struct timer_node{
    int fd; // socket描述符
    TIME_STAMP expires; // 定时器生效的绝对时间
    TIME_OUT_CALl_BACK cb;// 回调函数
    bool operator<(const timer_node& t){
        return expires < t.expires;
    }
};

// 最小堆是指每个结点的值都小于或等于其子结点的值的完全二叉树

// 最小堆-定时器
class heap_timer {

public:

    heap_timer();

    ~heap_timer();

    //  调整指定定时器超时时间
    void adjust(int fd,int new_expires);

    // 添加结点-client socket fd-超时时间-回调函数
    void add(int fd,int timeout,const TIME_OUT_CALl_BACK & cb);

    // 执行定时器对应的回调函数——执行任务-超时了调用。
    void do_work(int fd);

    // 清空时间堆
    void clear();

    // 检查堆中是否有超时的定时器
    void tick();

    //  删除堆顶定时器
    void pop();

    // 下一个要超时的定时器还有多少毫秒
    int get_next_tick();

private:

    // 删除索引为index的定时器
    void del(size_t index);

    // 向上调整
    void sift_up(size_t i);

    // 向下调整-确保每个结点有最小堆的性质，要调整的定时器索引，堆数组结尾下标。
    bool sift_down(size_t index,size_t n);

    // 交换堆内两个结点的位置，用于调整对时间堆的调整。
    void swap_node(size_t i,size_t j);


private:

    // 堆用数组实现
    std::vector<timer_node> m_heap;

    // 哈希表中的映射——<客户端socket fd,该fd在m_heap中的下标>
    std::unordered_map<int,size_t> m_ref;
};


#endif //TINYWEBSERVER_HEAP_TIMER_H
