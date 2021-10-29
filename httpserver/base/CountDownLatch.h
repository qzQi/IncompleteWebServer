

#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

/*
一个倒计时门栓，一个很常用的封装,mutex与cond常用的搭配
1、主线程等待所有线程池线程初始化完毕
2、所以线程等待父线程发布起跑命令
最后写个线程池与阻塞队列测试下。
*/

class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);
  ~CountDownLatch()=default;

  void wait();

  void countDown();
//   倒计时

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_ ;
  int count_ ;
};

#endif  // BASE_COUNTDOWNLATCH_H
