//
// Created by xuanxuan on 2023/2/6.
//

#include "../log/log.h"
#include "../thread_pool/thread_pool.h"
#include <features.h>

void test_log(){
    int cnt = 0;
    int level = 0;
    log::instance()->init(level,"./test_log1",".log",0);// 空
    for(level = 3;level >= 0;level--){
        log::instance()->set_level(level);
        for(int j = 0; j <10000;j++) {
            for(int i = 0; i < 4; i++){
                LOG_BASE(i,"%s 111111111 %d ============= ", "Test", cnt++);
            }
        }
    }
    cnt = 0;
    log::instance()->init(level,"./test_log2",".log",5000);// 5000最大行
    for(level = 0 ; level < 4;level++){
        log::instance()->set_level(level);
        for(int j = 0; j < 10000;j++){
            for(int i = 0; i < 4 ;i++){
                LOG_BASE(i,"%s 222222222 %d ============= ", "Test", cnt++);
            }
        }
    }
}


void thread_log_task(int i,int cnt){
    for(int j = 0; j < 10000;j++){
        LOG_BASE(i,"PID:[%04d]======= %05d ========= ", gettid(), cnt++);
    }
}

void test_thread_pool(){
    log::instance()->init(0,"./test_thread_pool",".log",5000);
    thread_pool threadpool(6);
    for(int i = 0; i < 18 ;i++){
        threadpool.add_task(std::bind(thread_log_task,i % 4,i * 10000));
    }
    getchar();// 好像是为了结局缓冲区溢出， core dump
}

int main(void){
    test_log();
    test_thread_pool();
    return 0;
}