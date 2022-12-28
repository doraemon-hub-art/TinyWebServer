// 
// Created by xuanxuan on 2022/11/28.
//

#ifndef TINYWEBSERVER_SQL_CONN_RAII_H
#define TINYWEBSERVER_SQL_CONN_RAII_H

#include "sql_conn_pool.h"

// RAII-资源构造即初始化，在对象析构时释放
class sql_conn_raii{
public:
    sql_conn_raii(MYSQL** sql,sql_conn_pool* conn_pool){
        assert(conn_pool);
        *sql = conn_pool->get_conn();
        m_sql = *sql;
        m_conn_pool = conn_pool;
    }

    ~sql_conn_raii(){
        if(m_sql){
            m_conn_pool->free_conn(m_sql);// 释放，放回连接池中。
        }
    }

private:
    // 数据库链接
    MYSQL* m_sql;

    // 数据库连接池-保存下来用于释放
    sql_conn_pool* m_conn_pool;
};


#endif //TINYWEBSERVER_SQL_CONN_RAII_H
