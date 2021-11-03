#ifndef CHANNEL_H
#define CHANNEL_H
/*
我们通过IO复用拿到感兴趣的fd文件描述符，在我们使用C的时候直接在过程里面通过判断
fd来为期分配对应的事件，这里我们通过对fd的抽象提供一个Channel类。Channel保存了fd
以及其所感兴趣的IO事件，以及对事件的处理程序。
但是muduo是一个通用的，我们这里对http做一些特化。         
需要适应poll来轮询的我们都让他拥有一个channel，比如Server的eventfd，HttpData里的

*/

#include <sys/epoll.h>
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class HttpData;

class Channel {
 private:
  typedef std::function<void()> CallBack;
  EventLoop *loop_;
  int fd_;
  __uint32_t events_;
  __uint32_t revents_;
  __uint32_t lastEvents_;

  // 对tcp连接的销毁起作用
  std::weak_ptr<HttpData> holder_;

 private:
  int parse_URI();
  int parse_Headers();
  int analysisRequest();

  CallBack readHandler_;
  CallBack writeHandler_;
  CallBack errorHandler_;
  CallBack connHandler_;

 public:
  Channel(EventLoop *loop);
  Channel(EventLoop *loop, int fd);
  ~Channel();
  int getFd();
  void setFd(int fd);

  void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
  std::shared_ptr<HttpData> getHolder() {
    std::shared_ptr<HttpData> ret(holder_.lock());
    return ret;
  }

  void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }
  void setWriteHandler(CallBack &&writeHandler) {
    writeHandler_ = writeHandler;
  }
  void setErrorHandler(CallBack &&errorHandler) {
    errorHandler_ = errorHandler;
  }
  void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

  void handleEvents() {
    events_ = 0;
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
      events_ = 0;
      return;
    }
    if (revents_ & EPOLLERR) {
      if (errorHandler_) errorHandler_();
      events_ = 0;
      return;
    }
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
      handleRead();
    }
    if (revents_ & EPOLLOUT) {
      handleWrite();
    }
    handleConn();
  }
  void handleRead();
  void handleWrite();
  void handleError(int fd, int err_num, std::string short_msg);
  void handleConn();

  void setRevents(__uint32_t ev) { revents_ = ev; }

  void setEvents(__uint32_t ev) { events_ = ev; }
  __uint32_t &getEvents() { return events_; }

  bool EqualAndUpdateLastEvents() {
    bool ret = (lastEvents_ == events_);
    lastEvents_ = events_;
    return ret;
  }

  __uint32_t getLastEvents() { return lastEvents_; }
};

typedef std::shared_ptr<Channel> SP_Channel;

#endif