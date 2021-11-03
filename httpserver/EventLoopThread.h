#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

/*
方便的创建一个IO线程（也就是拥有eventloop）的线程
*/

#include "EventLoop.h"
#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/Thread.h"
#include "base/noncopyable.h"


class EventLoopThread : noncopyable {
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();
  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
};

#endif