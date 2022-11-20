cmake_minimum_required(VERSION 3.22)
project(TinyWebServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(TinyWebServer main.cpp log/log.cpp log/log.h epoller/epoller.cpp epoller/epoller.h server/server.cpp server/server.h block_queue/block_queue.h lock/locker.cpp lock/locker.h http/http_conn.cpp http/http_conn.h buffer/buffer.cpp buffer/buffer.h log/log.cpp log/log.h)
