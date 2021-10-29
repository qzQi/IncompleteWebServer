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