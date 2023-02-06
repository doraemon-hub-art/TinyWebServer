#include <unistd.h>
#include "server/server.h"

int main() {

    // 数据库名称webserver
    // 表明user
    // 测试账号username : test password: 123
    // 数据库用户名: root
    // 数据库密码: 123

    // 数据库端口3306
    // WebServer服务器开放端口9006

    char buffer[256];
    getcwd(buffer, 256);
    printf("The current directory is: %s\n", buffer);

    server webserver(
            9006,3,60000, false,// 端口，fd模式，超时时间，优雅退出
            3306,"root","zyx020806","webserver",// mysql配置
            12,6,false,1,1024);// 连接池数量，线程池数量，日志开关，日志等级，日志异步队列容量
    webserver.start();
    return 0;
}

