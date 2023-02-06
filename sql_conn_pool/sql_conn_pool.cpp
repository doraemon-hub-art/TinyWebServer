// 
// Created by xuanxuan on 2022/11/22.
//

#include "sql_conn_pool.h"

sql_conn_pool::sql_conn_pool() {
    m_user_count = 0;
    m_free_count = 0;
}

sql_conn_pool::~sql_conn_pool() {
    close_pool();// 关闭连接池
}

sql_conn_pool* sql_conn_pool::instance() {
    static sql_conn_pool conn_pool;
    return &conn_pool;
}

void sql_conn_pool::init(const char *host,
                         int port,
                         const char *user,
                         const char *pwd,
                         const char *db_name,
                         int con_size = 10) {
    assert(con_size > 0);// 合法变量
    for(int i = 0; i < con_size;i++){//根据个数创建数据库连接
        MYSQL* sql = nullptr;
        sql = mysql_init(sql);
        if(!sql){// 创建失败
            LOG_ERROR("MySQL init error!");
            assert(sql);
        }
        // 开始连接
        sql = mysql_real_connect(sql,host,user,pwd,db_name,port, nullptr,0);
        if(!sql){
            LOG_ERROR("MySQL connet error!");
        }
        m_conn_que.push(sql);// 存入连接队列
    }
    m_max_conn = con_size;// 最大连接数
    // 初始化信号量，初始值为最大连接数
    sem_init(&m_sem_id,0,m_max_conn);
}

MYSQL* sql_conn_pool::get_conn() {
    MYSQL* sql = nullptr;
    if(m_conn_que.empty()){// 连接池空了
        LOG_WARN("MySQL connect pool busy!")
        return nullptr;
    }
    sem_wait(&m_sem_id);// 尝试获取信号量 & 信号量-1
    {// 临界区
        std::lock_guard<std::mutex>locker(m_mutex);// 上锁
        sql = m_conn_que.front();// 取出来最前面的
        m_conn_que.pop();// 出队列
    }
    return sql;
}

void sql_conn_pool::free_conn(MYSQL* sql) {
    assert(sql);
    std::lock_guard<std::mutex>locker(m_mutex);
    m_conn_que.push(sql);
    sem_post(&m_sem_id);// 信号量+1
}

void sql_conn_pool::close_pool() {
    std::lock_guard<std::mutex>locker(m_mutex);
    while(!m_conn_que.empty()){// 将连接池中的每个链接取出并关闭
        MYSQL* sql = m_conn_que.front();
        m_conn_que.pop();
        mysql_close(sql);
    }
    mysql_library_end();// 撤掉初始化的mysql执行环境，彻底关闭
}

int sql_conn_pool::get_free_conn_count() {
    std::lock_guard<std::mutex>locker(m_mutex);
    return m_conn_que.size();
}