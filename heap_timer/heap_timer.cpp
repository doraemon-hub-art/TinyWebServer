// 
// Created by xuanxuan on 2022/11/20.
//

#include "heap_timer.h"

heap_timer::heap_timer() {
    m_heap.reserve(64);// 可容纳64个元素-定时器
}

heap_timer::~heap_timer() {
    clear();// 清空时间堆
}

void heap_timer::swap_node(size_t i, size_t j) {
    // 范围是否合法
    assert( i >= 0 && i < m_heap.size());
    assert( j >= 0 && j < m_heap.size());
    // 交换
    std::swap(m_heap[i],m_heap[j]);
    // 哈希映射
    m_ref[m_heap[i].fd] = i;
    m_ref[m_heap[j].fd] = j;
}

void heap_timer::sift_up(size_t i) {
    assert( i >= 0 && i < m_heap.size());// 范围是否合法

    size_t j = (i - 1) / 2;// 父结点

    while ( j >= 0 ){
        if(m_heap[j] < m_heap[i]){// 父结点已经小于当前结点了，终止
            break;
        }
        // 交换，继续往上移动。
        swap_node(i,j);
        i = j;
        j = ( i - 1 ) / 2;
    }
}

bool heap_timer::sift_down(size_t index, size_t n) {
    assert( index >= 0 && index < m_heap.size());// 检查范围
    assert( n >= 0 && n <= m_heap.size());

    size_t i = index;
    size_t j = i * 2 + 1;// 左孩子 +2是右孩子
    while(j < n){// 往下找孩子的时候，索引小于n，因为可能只有一个孩子，嘻嘻。n应该是堆数组元素个数。
        if(j + 1 < n && m_heap[j+1] < m_heap[j])j++;// 找右孩子，更小的孩子
        if(m_heap[i] < m_heap[j])break;// 父结点值小于孩子结点值，终止
        swap_node(i,j);// 交换位置，相当于index结点下沉了。
        i = j;// 更新当前父结点位置下标
        j = i * 2 + 1;// 找左孩子
    }

    //  下沉操作，成功进行向下调整后，当前i 肯定大于原来的位置index
    if(i > index){
        return true;
    }else{// 向下调整失败
        return false;
    }
}

void heap_timer::add(int fd, int timeout, const  TIME_OUT_CALl_BACK & cb) {
    assert( fd >= 0);// 合法

    size_t i;// 数组中索引
    if(m_ref.count(fd) == 0){// 不存在该定时器
        // 新节点，堆尾插入，调整堆
        i = m_heap.size();
        m_ref[fd] = i;
        // 这么构造需要注意:在该结构体定义的时候，类内成员变量的定义顺序。
        m_heap.push_back({fd,CLOCK::now() + MS(timeout),cb});
        sift_up(i);
    }else{// 已有该结点，调整堆，重新设置相关定时器参数
        i = m_ref[fd];
        // 当前时间+一个时间长度ms
        m_heap[i].expires = CLOCK::now() + MS(timeout);//
        m_heap[i].cb = cb;
        // 先向下调整，失败则向上调整
        if(!sift_down(i,m_heap.size())){// 参数控制范围待确定
            sift_up(i);
        }
    }
}

void heap_timer::do_work(int fd) {
    // 删除指定id，并触发回调函数
    if(m_heap.empty() || m_ref.count(fd) == 0){// 不存在该id
        return ;
    }
    size_t index = m_ref[fd];// 拿到对应client socket fd在堆数组中的下标
    timer_node node = m_heap[index];// 拿到对应定时器
    node.cb();// 执行对应回调函数
    del(index);// 删除该定时器
}

void heap_timer::del(size_t index) {
    assert(!m_heap.empty() && index >= 0 && index < m_heap.size());// 范围是否合法

    // 将要删除的结点换到队尾，然后调整堆

    size_t n = m_heap.size() - 1;// 最后一个定时器的下标
    assert(index <= n);

    if(index < n){
        swap_node(index,n);
        // 这里传n好像错了，应该是n+1，没传错应该。
        if(!sift_down(index,n)){//向下调整失败，尝试向下调整。确保位置放到正确的位置。
            sift_up(index);
        }
    }

    // 队尾元素删除
    m_ref.erase(m_heap.back().fd);// 在哈希表映射中删除
    m_heap.pop_back();// 删除
}

void heap_timer::adjust(int fd, int new_expires) {
    // 调整指定fd的结点
    assert(!m_heap.empty() && m_ref.count(fd) > 0);// 堆中存在指定定时器

    // 更新绝对超时时间，时间只会更往后，所以只需要往下调整就OK，因为上面的父结点的绝对时间肯定小于这个超时时间。
    m_heap[m_ref[fd]].expires = CLOCK ::now() + MS(new_expires);

    // 向下调整
    sift_down(m_ref[fd],m_heap.size());
}

void heap_timer::pop() {
    assert(!m_heap.empty());// 不为空
    del(0);
}

void heap_timer::tick() {
    if(m_heap.empty()){ // 空的
        return ;
    }
    while(!m_heap.empty()){
        timer_node node = m_heap.front();// 取到堆顶元素
        // 小顶堆，堆顶不超时的，就直接break，无需再验证后面的了。
        // 判断毫秒差
        if(std::chrono::duration_cast<MS>(node.expires - CLOCK::now()).count() > 0){
            break;
        }
        // 超时啦
        node.cb();// 执行回调函数
        pop();// 弹出堆顶
    }
}

void heap_timer::clear() {
    m_ref.clear();
    m_heap.clear();
}

int heap_timer::get_next_tick() {
    tick();// 检查超时的定时器
    size_t res = -1;
    if(!m_heap.empty()){// 堆数组非空
        // 毫秒差
        res = std::chrono::duration_cast<MS>(m_heap.front().expires - CLOCK::now()).count();
        if(res < 0){// 已经超时
            res = 0;
        }
    }
    return res;
}
