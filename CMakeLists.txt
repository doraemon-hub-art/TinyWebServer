cmake_minimum_required(VERSION 3.22)
project(TinyWebServer)



set(CMAKE_CXX_STANDARD 14)

add_executable(TinyWebServer main.cpp log/log.cpp log/log.h epoller/epoller.cpp epoller/epoller.h server/server.cpp server/server.h block_queue/block_queue.h http/http_conn.cpp http/http_conn.h buffer/buffer.cpp buffer/buffer.h log/log.cpp log/log.h heap_timer/heap_timer.cpp heap_timer/heap_timer.h)
target_link_libraries(TinyWebServer pthread) # 链上thread库

