#include "Thread.h"
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include "CurrentThread.h"
 

#include <iostream>
using namespace std;

namespace CurrentThread {
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "default";
}

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}


// 在头文件提出的那个问题，ThreadData即使线程Thread对象不在了线程的相关信息
// 仍然保存
struct ThreadData {
//   using Thread::ThreadFunc;
  typedef Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  string name_;
  pid_t* tid_;
  CountDownLatch* latch_;

  ThreadData(const ThreadFunc& func, const string& name, pid_t* tid,
             CountDownLatch* latch)
      : func_(func), name_(name), tid_(tid), latch_(latch) {}

  void runInThread() {
    *tid_ = CurrentThread::tid();
    tid_ = NULL;
    latch_->countDown();
    latch_ = NULL;

    CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
    prctl(PR_SET_NAME, CurrentThread::t_threadName);

    func_();
    CurrentThread::t_threadName = "finished";
  }
};

void* startThread(void* obj) {
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}

Thread::Thread(ThreadFunc func, const string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(0),
      func_(std::move(func)),
      name_(n),
      latch_(1) {
  setDefaultName();
}

Thread::~Thread() {
  if (started_ && !joined_) pthread_detach(pthreadId_);
}
 
void Thread::setDefaultName() {
  if (name_.empty()) {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread");
    name_ = buf;
  }
}
// 创建线程对象后应该尽快使用start，
void Thread::start() {
  assert(!started_);
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
  if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
    started_ = false;
    delete data;
  } else {
    latch_.wait();
    // 等待线程运行
    assert(tid_ > 0);
  }
}

int Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}