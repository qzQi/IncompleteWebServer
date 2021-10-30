#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#pragma once
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "Mutex.h"
#include "noncopyable.h"


class Condition : noncopyable
{
private:
    MutexLock &mutex;
    pthread_cond_t cond;

public:
    explicit Condition(MutexLock& mutex_):mutex(mutex_)
    {
        pthread_cond_init(&cond,NULL);
    }
    ~Condition(){
        pthread_cond_destroy(&cond);
    }

    void wait()
    {
        pthread_cond_wait(&cond,mutex.GetMutex());
    }

// 超时的话返回true，否则返回false
    bool waitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }

    void notify()
    {
        pthread_cond_signal(&cond);
    }
    void notifyAll()
    {
        pthread_cond_broadcast(&cond);
    }
};

#endif