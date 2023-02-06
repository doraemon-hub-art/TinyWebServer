// 
// Created by xuanxuan on 2022/11/29.
//

#include "http_request.h"


const std::unordered_set<std::string>http_request::m_default_html{
    "/index","/register","/login","/welcome","/video","/picture"
};

const std::unordered_map<std::string,int>http_request::m_default_html_tag{
        {"/register.html",0},{"/login.html",1}
};

http_request::http_request() {
    init();
}


void http_request::init() {
    m_method = m_body = m_version = m_body = "";
    m_state = CHECK_STATE_REQUEST_LINE;// 初始化为解析请求行
    m_header.clear();
    m_post.clear();
}

bool http_request::is_keep_alive() const {
    if(m_header.count("Connection") == 1){
        if(m_header.find("Connection")->second == "keep-alive" && m_version == "1.1"){
            return true;
        }
    }
    return false;
}

bool http_request::parse(buffer &buffer) {
    const char CRLF[] = "\r\n";
    if(buffer.read_able_bytes() <= 0){
        return false;
    }
    // 请求行还没读完并且状态未到检查完的状态
    while(buffer.read_able_bytes() && m_state != CHECK_STATE_FINISH){
        // 定位到指定字符，返回其指针
        const char* line_end = std::search(buffer.peek(),buffer.begin_write_const(),
                                           CRLF,CRLF+2);
        std::string line(buffer.peek(),line_end);// 截取到\r\n之前的字符
        switch (m_state) {// 有限状态机
            case CHECK_STATE_REQUEST_LINE:{
                if(!parse_request_line(line)){
                    return false;
                }
                parse_path();// 解析要访问的文件名
                break;
            }
            case CHECK_STATE_HEADERS:{
                parse_header(line);
                if(buffer.read_able_bytes() <= 2){// 继续读取请求
                    m_state = CHECK_STATE_FINISH;// 状态转移
                }
                break;
            }
            case CHECK_STATE_BODY:{
                parse_body(line);
                break;
            }
            default:
                break;
        }
        if(line_end == buffer.begin_write()){// 读取进来的数据已经检测完毕啦
            break;
        }
        buffer.retrieve_until(line_end + 2);// 更新缓冲区中读指针位置
    }
    LOG_DEBUG("[%s],[%s],[%s]",m_method.c_str(),m_path.c_str(),m_version.c_str());
    return true;
}

void http_request::parse_path() {
    if(m_path == "/"){
        m_path = "/index.html";
    }else{
        for(auto& item:m_default_html){
            if(item == m_path){
                m_path += ".html";
                break;
            }
        }
    }
}

bool http_request::parse_request_line(const std::string &line) {
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch sub_match;
    if(std::regex_match(line,sub_match,patten)){// 成功捕获
        m_method = sub_match[1];
        m_path = sub_match[2];
        m_version = sub_match[3];
        m_state = CHECK_STATE_HEADERS;// 状态转移为分析请求头
        return true;
    }
    LOG_ERROR("RequestLine Error");
    return false;
}

void http_request::parse_header(const std::string &line) {
    std::regex patten("^([^:]*): ?(.*)$");
    std::smatch sub_match;
    if(std::regex_match(line,sub_match,patten)){
        m_header[sub_match[1]] = sub_match[2];
    }else{
        m_state = CHECK_STATE_BODY;// 状态转移
    }
}

void http_request::parse_body(const std::string &line) {
    m_body = line;
    parse_post();
    m_state = CHECK_STATE_FINISH;
    LOG_DEBUG("Body:%s, len:%d",line.c_str(),line.size());
}

int http_request::conver_hex(char ch) {
    if(ch >= 'A' && ch <= 'F')return ch -'A' + 10;
    if(ch >= 'a' && ch <= 'f')return ch -'a' + 10;
    return ch;
}

void http_request::parse_post() {
    if(m_method == "POST" && m_header["Content-Type"] == "application/x-www-form-urlencoded") {
        parse_from_urlen_coded();
        if(m_default_html_tag.count(m_path)){//
            int tag = m_default_html_tag.find(m_path)->second;// 取到对应标志
            LOG_DEBUG("Tag:%d",tag);
            if(tag == 0 || tag == 1){// 登录或者注册
                bool is_login = (tag == 1);// 是否是登录请求
                if(user_verify(m_post["username"],m_post["password"],is_login)){// 身份验证-登录
                    m_path = "/welcome.html";
                }else{
                    m_path = "/error.html";
                }
            }
        }
    }
}

void http_request::parse_from_urlen_coded() {
    if(m_body.size() == 0){
        return;
    }

    /**/
    std::cout<<m_body<<std::endl;
    /**/

    std::string key;
    std::string value;
    int num = 0;
    int n = m_body.size();
    int i = 0;
    int j = 0;

    for(;i < n;i++){
        char ch = m_body[i];
        switch (ch) {
        case '=':
            key = m_body.substr(j,i-j);
            j = i + 1;
            break;
        case '+':
            m_body[i] = ' ';
            break;
        case '%':
            num = conver_hex(m_body[i+1])*16 + conver_hex(m_body[i + 2]);
            m_body[i + 2] = num % 10 + '0';
            m_body[i +1] = num / 10 + '0';
            i += 2;
            break;
        case '&':
            value = m_body.substr(j,i - j);
            j = i + 1;
            m_post[key] = value;
            LOG_DEBUG("%s = %s",key.c_str(),value.c_str());
            break;
        default:
            break;
        }
    }
    assert( j <= i);
    if(m_post.count(key) == 0 && j < i){
        value = m_body.substr(j,i - j);
        m_post[key] = value;
    }
}

bool http_request::user_verify(const std::string &name, const std::string &pwd, bool is_login) {
    if(name == "" || pwd == ""){// 账号密码为空
        return false;
    }
    LOG_INFO("verify name:%s pwd:%s",name.c_str(),pwd.c_str());
    MYSQL* sql;
    sql_conn_raii(&sql,sql_conn_pool::instance());// 从数据连接池中取出一个连接
    assert(sql);

    bool flag = false;// 注册标志
    unsigned  int j =  0;
    char order[256] = {0};
    MYSQL_FIELD* fields = nullptr;
    MYSQL_RES* res = nullptr;

    if(!is_login){// 注册
        flag = true;
    }

    // 查询用户及密码
    snprintf(order,256,
             "SELECT username, password FROM user WHERE username='%s' LIMIT 1",
             name.c_str());
    LOG_DEBUG("%s",order);

    if(mysql_query(sql,order)){// 查询失败
        mysql_free_result(res);
        return false;
    }

    res = mysql_store_result(sql);
    j = mysql_num_fields(res);// 结果集列数
    fields = mysql_fetch_fields(res);// 取得列的信息

    while(MYSQL_ROW row = mysql_fetch_row(res)){// 获取结果集每一行数据
        LOG_DEBUG("MYSQL ROW: %s %s",row[0],row[1]);
        std::string password(row[1]);// 获取查询到的对应用户名密码
        if(is_login){// 登录
            if(pwd == password){// 密码正确
                flag = true;
            }else{// 密码错误
                flag = false;
                LOG_DEBUG("pwd error!");
            }
        }else{// 注册
            flag = false;
            LOG_DEBUG("user used!");// 用户名已经使用，查到了，说明已经使用过了。
        }
    }

    mysql_free_result(res);

    // 注册——执行条件，注册 && flag == true
    if(!is_login && flag == true){
        LOG_DEBUG("regirster!");
        bzero(order,256);
        snprintf(order,256,"INSERT INTO user(username, password) VALUES('%s','%s')",
                 name.c_str(),pwd.c_str());
        LOG_DEBUG("%s",order);
        if(mysql_query(sql,order)){// 注册失败
            LOG_DEBUG("insert error!");
            flag = false;
        }
        flag = true;
    }

    sql_conn_pool::instance()->free_conn(sql);// 将数据库链接放回连接池
    LOG_DEBUG("user verify success(finished)");//
    return flag;
}

std::string& http_request::path() {
    return m_path;
}

std::string http_request::path()const {
    return  m_path;
}

std::string http_request::method() const {
    return m_method;
}

std::string http_request::version() const {
    return m_version;
}

std::string http_request::get_post(const std::string &key) const {
    assert(key != "");
    if(m_post.count(key) == 1){
        return m_post.find(key)->second;
    }
    return "";
}

std::string http_request::get_post(const char *key) const {
    assert(key != nullptr);
    if(m_post.count(key) == 1){
        return m_post.find(key)->second;
    }
    return "";
}