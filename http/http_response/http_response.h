// 
// Created by xuanxuan on 2022/11/30.
//

#ifndef TINYWEBSERVER_HTTP_RESPONSE_H
#define TINYWEBSERVER_HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>   // stat
#include <sys/mman.h>   // mmap

#include "../../buffer/buffer.h"
#include "../../log/log.h"

class http_response {
public:

    http_response();

    ~http_response();

    // 初始化
    void init(const std::string& src_dir,
                    std::string& path,
                    bool is_keep_alive = false,
                    int code  = -1);

    // 作出响应，调用下方几个模块函数
    void make_response(buffer& buff);

    // 解除文件映射
    void unmap_file();

    // 获取指向映射区域的指针-成员变量m_mmfile
    char* file();

    // 文件大小
    size_t file_len()const;

    // 访问错误，给出对应的错误响应
    void error_content(buffer& buff,std::string message);

    // 获取当前状态码
    int code()const;

private:
    // 添加响应状态行
    void add_state_line(buffer& buff);

    // 添加响应头
    void add_header(buffer& buff);

    // 添加响应内容
    void add_content(buffer& buff);

    // 获取请求文件的信息,保存在m_mmfile_stat中
    void error_html();

    // 获取请求的文件类型
    std::string get_file_type();

private:

    // 状态码
    int m_code;

    // 是否为长连接
    bool m_is_keep_alive;

    // 响应文件名
    std::string m_path;

    // 资源文件夹-响应文件所在位置
    std::string m_src_dir;

    // 指向映射区的指针
    char* m_mmfile;

    // 保存响应文件的信息
    struct stat m_mmfile_stat;

    // 添加响应类型后缀
    static const std::unordered_map<std::string,std::string> m_suffix_type;

    // http状态码-及对应意思
    static const std::unordered_map<int,std::string>m_code_status;

    // 错误码与之对应的响应文件
    static const std::unordered_map<int,std::string>m_code_path;
};


#endif //TINYWEBSERVER_HTTP_RESPONSE_H
