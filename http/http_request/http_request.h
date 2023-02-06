// 
// Created by xuanxuan on 2022/11/29.
//

#ifndef TINYWEBSERVER_HTTP_REQUEST_H
#define TINYWEBSERVER_HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex> // 正则表达式库
#include <cerrno>
#include <mysql/mysql.h>// mysql

#include "../../buffer/buffer.h"
#include "../../log/log.h"
#include "../../sql_conn_pool/sql_conn_pool.h"
#include "../../sql_conn_pool/sql_conn_raii.h"

class http_request {
public:
    // 主状态机状态
    enum PARSE_STATE{
        // 检查请求行
        CHECK_STATE_REQUEST_LINE,
        // 检查请求头
        CHECK_STATE_HEADERS,
        // 检查请求体
        CHECK_STATE_BODY,
        // 检查完成
        CHECK_STATE_FINISH
    };

    // 服务器处理http请求的结果
    enum HTTP_CODE{
        // 请求不完整，需要继续读取客户数据
        NO_REQUEST,
        // 获得了一个完整的客户请求
        GET_REQUEST,
        // 客户请求有语法错误
        BAD_REQUEST,
        // 客户对资源没有足够的权限访问
        FORBIDDEN_REQUEST,
        // 请求文件
        FILE_REQUEST,
        // 服务器内部错误
        INTERNAL_ERROR,
        // 客户端已经关闭连接
        CLOSED_CONNECTION
    };

    http_request();

    ~http_request() = default;

    // 初始化
    void init();

    // 解析——有限状态机-内部进行状态转移
    bool parse(buffer& buffer);

    // 获取要访问的html文件
    std::string path()const;

    // 获取要访问的html文件
    std::string& path();

    // 获取请求方法
    std::string method()const;

    // 获取http版本
    std::string version()const;

    //
    std::string get_post(const std::string &key)const;

    //
    std::string get_post(const char* key)const;

    // 是否为长连接
    bool is_keep_alive()const;

private:
    // 分析请求行
    bool parse_request_line(const std::string& line);

    // 分析头部字段
    void parse_header(const std::string& line);

    // 分析请求体
    void parse_body(const std::string& line);

    // 解析获取要访问的html文件名
    void parse_path();

    // 分析post请求
    void parse_post();

    //
    void parse_from_urlen_coded();

    // 用户验证
    static bool user_verify(const std::string& name,const std::string& pwd,bool is_login);

private:
    // 主状态机状态
    PARSE_STATE m_state;

    // http请求方法
    std::string m_method;

    // 要访问的html文件名,xx.html
    std::string m_path;

    // http版本-1.1
    std::string m_version;

    // 请求体
    std::string m_body;

    // 保存当前连接的请求头
    std::unordered_map<std::string,std::string>m_header;

    //
    std::unordered_map<std::string,std::string>m_post;

    // 可访问的html路径
    static const std::unordered_set<std::string> m_default_html;

    // 页面对应的post请求-登录-注册
    static const std::unordered_map<std::string,int>m_default_html_tag;

    //
    static int conver_hex(char ch);


};


#endif //TINYWEBSERVER_HTTP_REQUEST_H
