cmake_minimum_required(VERSION 3.10.0)
project(unit_test)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/mysql)

add_executable(unit_test unit_test.cpp ../log/log.cpp  ../log/log.h  ../epoller/epoller.cpp  ../epoller/epoller.h  ../server/server.cpp  ../server/server.h  ../block_queue/block_queue.h  ../http/http_conn/http_conn.cpp  ../http/http_conn/http_conn.h  ../buffer/buffer.cpp  ../buffer/buffer.h    ../heap_timer/heap_timer.cpp  ../heap_timer/heap_timer.h  ../sql_conn_pool/sql_conn_pool.cpp  ../sql_conn_pool/sql_conn_pool.h  ../sql_conn_pool/sql_conn_raii.h  ../thread_pool/thread_pool.h  ../http/http_request/http_request.cpp  ../http/http_request/http_request.h  ../http/http_response/http_response.cpp  ../http/http_response/http_response.h)

target_link_libraries(unit_test pthread) # 链上thread库
target_link_libraries(unit_test mysqlclient)
