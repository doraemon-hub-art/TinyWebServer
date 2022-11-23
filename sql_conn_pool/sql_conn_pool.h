// 
// Created by xuanxuan on 2022/11/22.
//

#ifndef TINYWEBSERVER_SQL_CONN_POOL_H
#define TINYWEBSERVER_SQL_CONN_POOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"


class sql_conn_pool {
public:
    // 单例模式-获取实例
    static sql_conn_pool* instance();

    // 得到链接
    MYSQL *get_conn();

    // 释放链接
    void free_conn();

    //
    int get_free_conn_count();

    // 初始化
    void init(const char* host,
              int port,
              const char* user,
              const char* pwd,
              const char* db_name,
              int con_size);

    // 关闭连接池
    void close_pool();

private:

    // 私有化构造函数
    sql_conn_pool();

    // 析构
    ~sql_conn_pool();

    // 最大连接数量
    int m_max_conn;

    // 已使用连接数量
    int m_use_count;

    // 空闲连接数量
    int m_free_count;

    // 连接池容器
    std::queue<MYSQL*> m_conn_que;

    // 互斥锁
    std::mutex m_mutex;

    // 信号量
    sem_t m_sem_id;
};


#endif //TINYWEBSERVER_SQL_CONN_POOL_H
