// 
// Created by xuanxuan on 2022/11/30.
//

#include "http_response.h"

const std::unordered_map<std::string,std::string> http_response::m_suffix_type = {
        { ".html",  "text/html" },
        { ".xml",   "text/xml" },
        { ".xhtml", "application/xhtml+xml" },
        { ".txt",   "text/plain" },
        { ".rtf",   "application/rtf" },
        { ".pdf",   "application/pdf" },
        { ".word",  "application/nsword" },
        { ".png",   "image/png" },
        { ".gif",   "image/gif" },
        { ".jpg",   "image/jpeg" },
        { ".jpeg",  "image/jpeg" },
        { ".au",    "audio/basic" },
        { ".mpeg",  "video/mpeg" },
        { ".mpg",   "video/mpeg" },
        { ".avi",   "video/x-msvideo" },
        { ".gz",    "application/x-gzip" },
        { ".tar",   "application/x-tar" },
        { ".css",   "text/css "},
        { ".js",    "text/javascript "},
};

const std::unordered_map<int,std::string> http_response::m_code_status = {
        { 200, "OK" },
        { 400, "Bad Request" },
        { 403, "Forbidden" },
        { 404, "Not Found" },
};

const std::unordered_map<int,std::string>http_response::m_code_path = {
        { 400, "/400.html" },
        { 403, "/403.html" },
        { 404, "/404.html" },
};

http_response::http_response() {
    m_code = -1;
    m_path = "";
    m_src_dir = "";
    m_is_keep_alive = false;
    m_mmfile = nullptr;
    m_mmfile_stat = {0};
}

http_response::~http_response() {
    unmap_file();
}

void http_response::init(const std::string &src_dir, std::string &path, bool is_keep_alive, int code) {
    assert(src_dir != "");
    if(m_mmfile){// 已经映射了，先解除映射
        unmap_file();
    }
    m_code = code;
    m_is_keep_alive = is_keep_alive;
    m_path = path;
    m_src_dir = src_dir;
    m_mmfile = nullptr;
    m_mmfile_stat = {0};
}

void http_response::make_response(buffer &buff) {
    // 判断请求的资源文件
    if(stat((m_src_dir+m_path).data(),&m_mmfile_stat) < 0 || // 错误 || 是文件夹
            S_ISDIR(m_mmfile_stat.st_mode)){
        m_code = 404;
    }else if(!(m_mmfile_stat.st_mode & S_IROTH)){// 用户读权限，403错误权限不够
        m_code = 403;
    }else if(m_code == -1){
        m_code = 200;
    }

    error_html();
    add_state_line(buff);
    add_header(buff);
    add_content(buff);
}

char* http_response::file() {
    return m_mmfile;
}

size_t http_response::file_len() const {
    return m_mmfile_stat.st_size;
}

void http_response::error_html() {
    if(m_code_path.count(m_code) == 1){// 存在该状态码
        m_path = m_code_path.find(m_code)->second;// 对应的相应文件
        stat((m_src_dir + m_path).data(),&m_mmfile_stat);// 保存文件信息
    }
}

void http_response::add_state_line(buffer &buff) {
    std::string status;
    if(m_code_status.count(m_code) == 1){// 找到该状态码-在预设状态码中
        status = m_code_status.find(m_code)->second;
    }else{// 没找到-设为400异常请求
        m_code = 400;
        status = m_code_status.find(400)->second;
    }
    buff.append("HTTP/1.1 "+ std::to_string(m_code) + " "+status + "\r\n");
}

void http_response::add_header(buffer &buff) {
    buff.append("Connection: ");
    if(m_is_keep_alive){// 长连接
        buff.append("keep_alive\r\n");
        buff.append("keep-alive: max=6, timeout=120\r\n");
    }else{
        buff.append("close\r\n");
    }
    buff.append("Content-type: " + get_file_type() + "\r\n");
}

void http_response::add_content(buffer &buff) {
    int src_fd = open((m_src_dir+m_path).data(),O_RDONLY);// 打开对应为文件
    if(src_fd < 0){// 没找到对应文件
        error_content(buff,"File Not Found!");
        return;
    }

    // 将文件映射到内存，提高文件的访问速度，MAP_PRIVATE 建立一个写入时拷贝的私有映射
    LOG_DEBUG("file path %s",(m_src_dir + m_path).data());
    // 返回指向被映射区域的指针
    int* mm_ret = (int*) mmap(0,m_mmfile_stat.st_size,PROT_READ,MAP_PRIVATE,src_fd,0);

    if(*mm_ret == -1){// 失败时返回-1，强转成int后判断
        error_content(buff,"File Not Found");
        return ;
    }

    m_mmfile = (char*)mm_ret;
    close(src_fd);// 映射到内存中了，将文件关闭
    buff.append("Content-length: "+ std::to_string(m_mmfile_stat.st_size) + "\r\n\r\n");
}

int http_response::code() const {
    return m_code;
}

void http_response::unmap_file() {
    if(m_mmfile){// 非空，说明有映射
        munmap(m_mmfile,m_mmfile_stat.st_size);// 解除映射
        m_mmfile = nullptr;
    }
}

std::string http_response::get_file_type() {
    // 判断文件类型
    std::string::size_type idx = m_path.find_last_of('.');// 查找.最后一次出现的位置,unsigned int
    if(idx == std::string::npos){// 没找到
        return "text/plain";
    }
    std::string suffix = m_path.substr(idx);// 拿取后缀，判断请求的文件类型
    if(m_suffix_type.count(suffix) == 1){
        return m_suffix_type.find(suffix)->second;
    }
    return "text/plain";
}

void http_response::error_content(buffer &buff, std::string message) {
    std::string body;
    std::string status;

    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";

    if(m_code_status.count(m_code == 1)){
        status = m_code_status.find(m_code)->second;
    }else{// 不存在此状态码
        status = "Bad Request";
    }

    body += std::to_string(m_code) + " : " + status + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>TinyWebServer</em></body></html>";

    buff.append("Content-length: " + std::to_string(body.size()) + "\r\n\r\n");
    buff.append(body);
}



