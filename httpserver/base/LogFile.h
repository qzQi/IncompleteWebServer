/*
日志文件的管理：
计划支持自动的回滚日志文件，rollFile          

如果不支持日志回滚的话这个实现不实现都无所谓，
直接在Logger指定文件就够了，        


emmm，目前没有实现回滚功能，这个可有可无，待添加......
*/

#ifndef BASE_LOG_FILE_h
#define BASE_LOG_FILE_H

#include <memory>
#include <string>
#include "FileUtil.h"
#include "Mutex.h"
#include "noncopyable.h"
// 待完成：添加一个日志回滚的功能。

class LogFile : noncopyable {
 public:
  // 每被append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
  LogFile(const std::string& basename, int flushEveryN = 1024);
  ~LogFile();

  void append(const char* logline, int len);
  void flush();
  bool rollFile();
//   实现日志文件的自动回滚

 private:
  void append_unlocked(const char* logline, int len);

  const std::string basename_;
  const int flushEveryN_;

  int count_;
  std::unique_ptr<MutexLock> mutex_;
  std::unique_ptr<AppendFile> file_;
};

#endif