#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

/*
提供给我们的Server使用，来创建多个IO线程，Server线程仅仅处理listen（acceptor）
socket就行了，已经建立的tcpconnection（也就是httpdata）它的所有请求我们在建立的时候
使用Round R 算法分给其他IO线程       

emmmm，我们的server模型使用Multiple Reactors就足够，我们的解析
http协议基本上没有计算任务QwQ。
*/

#include <memory>
#include <vector>
#include "EventLoopThread.h"
#include "base/Logging.h"
#include "base/noncopyable.h"


class EventLoopThreadPool : noncopyable {
 public:
  EventLoopThreadPool(EventLoop* baseLoop, int numThreads);

  ~EventLoopThreadPool() { LOG << "~EventLoopThreadPool()"; }
  void start();

  EventLoop* getNextLoop();

 private:
  EventLoop* baseLoop_;
  bool started_;
  int numThreads_;
  int next_;
  std::vector<std::shared_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};


#endif