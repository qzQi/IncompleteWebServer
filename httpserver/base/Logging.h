#ifndef BASE_LOGGING_H
#define BASE_LOGGING_H
/*
muduo里面的Logger十分的强大，通过一些set可以变为异步或是其他的设置     

这里我仅仅完成异步的日志吧，
*/


#include "./AsyncLogging.h"
#include "./LogStream.h"
#include <string>

class Logger {
 public:
  Logger(const char *fileName, int line);
  ~Logger();
  LogStream &stream() { return impl_.stream_; }

  static void setLogFileName(std::string fileName) { logFileName_ = fileName; }
  static std::string getLogFileName() { return logFileName_; }

 private:
  class Impl {
   public:
    Impl(const char *fileName, int line);
    void formatTime();

    LogStream stream_;
    int line_;
    std::string basename_;
  };
  Impl impl_;
  static std::string logFileName_;
};

#define LOG Logger(__FILE__, __LINE__).stream()
// 构造一个匿名的对象，一条log语句后就析构掉（写进stream的缓冲）

#endif