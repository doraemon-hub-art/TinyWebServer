## 项目说明

> - 使用C++实现的高性能Web服务器，经过压力测试可达上万QPS

>- 原项目地址——[WebServer](https://github.com/markparticle/WebServer)



***

## 功能

> - 利用IO多路复用技术Epoll与线程池实现的Reactor高并发模型。
> - 利用正则表达式和优先状态机解析HTTP请求报文，实现对静态资源的请求处理。
> - 对Vector进一步封装，实现可自动增长的缓冲区。
> - 使用Vector构建的小根堆，实现定时器，关闭超时的非活动连接，节省服务器资源。
> - 单例模式的日志系统，使用阻塞队列/缓冲池，实现双缓冲的异步/同步写入功能。监控服务器运行状态。
> - 使用单例模式，RAII机制实现了数据库连接池，减少数据库建立与关闭的开销。并实现用户登录与注册功能，将用户信息存入MySQL数据库中。
> - 添加压力测试，单元测试，测试服务器性能与模块状态。

## 开发环境

> - Ubuntu Server 20.04.1 (虚拟机内存8G,处理器2*2)
> - C++ 11
> - CMake  3.25.1
> - MySQL 8.0.32-0buntu0.22.04.1 for Linux on x86_64

## 编译

```bash
mkdir build
cd build
cmake ..
make
```

## MySQL数据库环境配置

```bash
# sudo apt install mysql-server -y

# sudo apt-get update
# sudo apt-get install libmysqlclient-dev
```

## 创建数据库表

```mysql

```

## 项目结构

```bash
├── CMakeFiles
├── CMakeLists.txt
├── Makefile
├── README.md
├── TinyWebServer
├── TinyWebServer.cbp
├── block_queue
│   └── block_queue.h
├── buffer
│   ├── buffer.cpp
│   └── buffer.h
├── epoller
│   ├── epoller.cpp
│   └── epoller.h
├── heap_timer
│   ├── heap_timer.cpp
│   └── heap_timer.h
├── http
│   ├── http_conn
│   │   ├── http_conn.cpp
│   │   └── http_conn.h
│   ├── http_request
│   │   ├── http_request.cpp
│   │   └── http_request.h
│   └── http_response
│       ├── http_response.cpp
│       └── http_response.h
├── log
│   ├── log.cpp
│   ├── log.h
│   └── logs
│       ├── 2023_02_06.log
│       └── test
├── main.cpp
├── resources
├── server
│   ├── server.cpp
│   └── server.h
├── sql_conn_pool
│   ├── sql_conn_pool.cpp
│   ├── sql_conn_pool.h
│   └── sql_conn_raii.h
├── thread_pool
│   └── thread_pool.h
├── unit_test
│   ├── CMakeLists.txt
│   ├── Makefile
│   ├── README.md
│   ├── cmake_install.cmake
│   ├── test_log1
│   │   ├── 2023_02_06-1.log
│   │   └── 2023_02_06.log
│   ├── test_log2
│   │   ├── 2023_02_06-1.log
│   │   └── 2023_02_06.log
│   ├── test_thread_pool
│   │   ├── 2023_02_06-1.log
│   │   ├── 2023_02_06-2.log
│   │   ├── 2023_02_06-3.log
│   │   └── 2023_02_06.log
│   ├── unit_test
│   └── unit_test.cpp
├── working_note
│   └── README.md
└── 跑项目的时候把服务器上的文件都删除，重新将本地文件同步上去。
```



***

## 压力测试
>- 经测试，本项目可达 1000+ QPS

>- 指令

```bash
# 注意根据实际情况修改ip和端口
./webbench-1.5/webbench -c 10000 -t 10 http://127.0.0.1:9006/
```

>- 需要注意查看服务器本机文件描述符限制
>  - 使用命令 查看——ulimit -n # 默认是1024
>  - 解除限制——详见: [linux文件描述符限制及使用详解](https://blog.csdn.net/guotianqing/article/details/82313996)
>    - 临时修改: ulimit -HSn 65536，只在当前shell窗口中有效
>    - 永久修改: 编辑——/etc/security/limits.conf
>
>

> - 修改前: 提示资源不足
>   - **还有可能是因为内存不足，我将虚拟机内存从4G给到8G后，问题解决。**

![image-20230206124949950](https://zyximagestorage.oss-cn-beijing.aliyuncs.com/blogimages/image-20230206124949950.png)

> - 修改后

![](https://zyximagestorage.oss-cn-beijing.aliyuncs.com/blogimages/20230206131623.png)

>- 测试相关补充——[怎样正确做 Web 应用的压力测试？ - 玩家翁伟的回答 - 知乎](https://www.zhihu.com/question/19867883/answer/391492300)

***

## 单元测试

> - 测试线程池功能与日志写入功能是否正常。

```bash
cd unit_test
cmake .
make
./unit_test
```

