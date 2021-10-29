#ifndef BASE_THREAD_H
#define BASE_THREAD_H

/*
学习muduo，这个类，我们也是基于对象的编程。
Thread对象的生命期不应该来影响我们的线程,
这个如何解决：muduo里面的解决方案非常好。
我们的线程应该找个堆数据持久的存在（
emmm，再次看貌似不是解决这个问题的，第一次还以为是解决这个问题。。。
Thread * th1 =new Thread(func_,name);
th1->start();
th1->join();
delete th1;
待完成：
*/

#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>//for syscall(SYS_gettid)

#include<functional>
#include<memory>
#include<string>

#include"Mutex.h"
#include"CountDownLatch.h" // wait for pthread_create
#include"noncopyable.h"
using std::string;

class Thread : noncopyable {
 public:
  typedef std::function<void()> ThreadFunc;
  explicit Thread(const ThreadFunc&, const std::string& name = std::string());
  ~Thread();
  void start();
  int join();
  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

 private:
  void setDefaultName();
  bool started_;
  bool joined_;
  pthread_t pthreadId_;
  pid_t tid_;
  ThreadFunc func_;
  std::string name_;
  CountDownLatch latch_;
};

#endif