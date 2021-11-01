#ifndef BASE_ASYNCLOGGING_H
#define BASE_ASYNCLOGGING_H

/*
给LogFile再加一层，其实没有这一层的话，我们的Logging已经基本完善了

这个的目的是实现异步的线程日志，这个负责日志的后端写入
*/
#include "./Mutex.h"
#include "./CountDownLatch.h"
#include "./Condition.h"
#include "./Thread.h"
#include "./noncopyable.h"
#include "./LogFile.h"
#include "./LogStream.h"
#include <vector>
#include <functional>
#include <memory>
#include <string>
 
class AsyncLogging : noncopyable {
 public:
  AsyncLogging(const std::string basename, int flushInterval = 2);
  ~AsyncLogging() {
    if (running_) stop();
  }
  void append(const char* logline, int len);

  void start() {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop() {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:
  void threadFunc();
  typedef FixedBuffer<kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef std::unique_ptr<Buffer> BufferPtr;
  const int flushInterval_;
  bool running_;
  std::string basename_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
  CountDownLatch latch_;
};

#endif