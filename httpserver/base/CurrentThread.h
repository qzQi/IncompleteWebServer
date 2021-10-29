#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H
 
#include <stdint.h>

namespace CurrentThread {
// __thread 每个线程专有的数据，存于线程栈，每个线程都有
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

void cacheTid();
inline int tid() {
    // GCC分支预测，表示t_cachedTid==0这个事件发生概率不大
  if (__builtin_expect(t_cachedTid == 0, 0)) {
    cacheTid();
  }
  return t_cachedTid;
}

inline const char* tidString()  // for logging
{
  return t_tidString;
}

inline int tidStringLength()  // for logging
{
  return t_tidStringLength;
}

inline const char* name() { return t_threadName; }

}

#endif