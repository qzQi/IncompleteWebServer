// 本机是4核 CPU，默认线程为4

#ifndef BASE_THREADPOOL_h
#define BASE_THREADPOOL_h
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"

#include<deque>
#include<vector>
#include<memory>
#include<functional>

using namespace std;


class ThreadPool : noncopyable
{
 public:
  typedef std::function<void ()> Task;

  explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb)
  { threadInitCallback_ = cb; }

  void start(int numThreads=4);
  void stop();
 
  const string& name() const
  { return name_; }

  size_t queueSize() const;

// 添加任务，超出最大任务就阻塞，等待notfull条件
  void run(Task f);

 private:
  bool isFull() const ;
  void runInThread();
  Task take();
//   线程池中的线程取任务，没任务的话，等待notempty信号

  mutable MutexLock mutex_;
  Condition notEmpty_ ;
  Condition notFull_ ;
  string name_;
  Task threadInitCallback_;
  std::vector<std::unique_ptr<Thread>> threads_;
  std::deque<Task> queue_ ;
  size_t maxQueueSize_;
  bool running_;
};

#endif