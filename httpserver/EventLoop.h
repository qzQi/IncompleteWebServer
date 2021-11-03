#ifndef EVENTLOOP_H
#define EVENTLOOP_H

/*
也是模仿muduo，这是对我们epoll的抽象，事件循环也是我们Reactor的关键结构
使用__thread关键字，灵活使用线程的局部存储，保证我们每个IO线程只有一个事件循环
EventLoop对象。      
这里面runInloop是关键结构，我觉得muduo设计的非常厉害。       
这里使用了eventfd来唤醒我们的loop（IO线程）
*/

#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "base/CurrentThread.h"
#include "base/Logging.h"
#include "base/Thread.h"


#include <iostream>
using namespace std;

class EventLoop {
 public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();
  void loop();
  void quit();
  void runInLoop(Functor&& cb);
  void queueInLoop(Functor&& cb);
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  void assertInLoopThread() { assert(isInLoopThread()); }
  void shutdown(shared_ptr<Channel> channel) { shutDownWR(channel->getFd()); }
  void removeFromPoller(shared_ptr<Channel> channel) {
    // shutDownWR(channel->getFd());
    poller_->epoll_del(channel);
  }
  void updatePoller(shared_ptr<Channel> channel, int timeout = 0) {
    poller_->epoll_mod(channel, timeout);
  }
  void addToPoller(shared_ptr<Channel> channel, int timeout = 0) {
    poller_->epoll_add(channel, timeout);
  }

 private:
  // 声明顺序 wakeupFd_ > pwakeupChannel_
  bool looping_;
  shared_ptr<Epoll> poller_;
  // eventfd唤醒IO线程使用
  int wakeupFd_;
  bool quit_;
  bool eventHandling_;
  mutable MutexLock mutex_;
  std::vector<Functor> pendingFunctors_;
  bool callingPendingFunctors_;
  const pid_t threadId_;
  // 对每个fd我们都对其抽象Channel，来和它所关心的事件绑定
  shared_ptr<Channel> pwakeupChannel_;

  void wakeup();
  void handleRead();
  void doPendingFunctors();
  void handleConn();
};


#endif