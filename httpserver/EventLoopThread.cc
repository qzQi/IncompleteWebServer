#include "EventLoopThread.h"
#include <functional>

EventLoopThread::EventLoopThread()
    : loop_(NULL),
      exiting_(false),
      thread_(bind(&EventLoopThread::threadFunc, this), "EventLoopThread"),
      mutex_(),
      cond_(mutex_) {}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  if (loop_ != NULL) {
    loop_->quit();
    thread_.join();
  }
}

EventLoop* EventLoopThread::startLoop() {
  assert(!thread_.started());
  thread_.start();
  {
    MutexLockGuard lock(mutex_);
    // 一直等到threadFun在Thread里真正跑起来
    while (loop_ == NULL) cond_.wait();
  }
  return loop_;
}

void EventLoopThread::threadFunc() {
  EventLoop loop;

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
//   对于我们的服务器程序，如果连IO线程都结束的话大概已经整改程序都die了。
  // assert(exiting_);
  loop_ = NULL;
}