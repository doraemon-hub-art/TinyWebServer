// 
// Created by xuanxuan on 2022/11/16.
//

#include "locker.h"


/***************************sem****************************/

sem::sem() {
    if(sem_init(&m_sem,0,0) != 0){
        throw std::exception();
    }
}

sem::sem(int num) {
    if(sem_init(&m_sem,0,num) != 0){
        throw std::exception();
    }
}

sem::~sem() {
    sem_destroy(&m_sem);
}

bool sem::wait() {
    if(sem_wait(&m_sem) == 0){
        return true;
    }else{
        return false;
    }
}

bool sem::post() {
    if(sem_post(&m_sem) == 0){
        return true;
    }else{
        return false;
    }
}

/***************************locker****************************/

locker::locker() {
    if(pthread_mutex_init(&m_mutex, nullptr) != 0){
        throw std::exception();
    }
}

locker::~locker() {
    pthread_mutex_destroy(&m_mutex);
}

bool locker::lock() {
    if(pthread_mutex_lock(&m_mutex) == 0){
        return true;
    }else{
        return false;
    }
}

bool locker::unlock() {
    if(pthread_mutex_unlock(&m_mutex) == 0){
        return true;
    }else{
        return false;
    }
}

pthread_mutex_t * locker::get() {
    return &m_mutex;
}

/***************************cond****************************/

cond::cond() {
    if(pthread_cond_init(&m_cond, nullptr) != 0){
        throw std::exception();
    }
}

cond::~cond() {
    pthread_cond_destroy(&m_cond);
}

bool cond::wait(pthread_mutex_t *m_mutex) {
    if(pthread_cond_wait(&m_cond,m_mutex) == 0){
        return true;
    }else{
        return false;
    }
}

bool cond::timewait(pthread_mutex_t *m_mutex, struct timespec t) {
    if(pthread_cond_timedwait(&m_cond,m_mutex,&t) == 0){
        return true;
    }else{
        return true;
    }
}

bool cond::signal() {
    if(pthread_cond_signal(&m_cond) == 0){
        return true;
    }else {
        return false;
    }
}