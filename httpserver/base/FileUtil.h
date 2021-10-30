/*
提供最底层的对文件的操作。
*/

#ifndef BASE_FILE_UTIL_H
#define BASE_FILE_UTIL_h

#include "noncopyable.h"
#include <string>
using std::string;

class AppendFile : noncopyable
{
private:
  size_t write(const char*logline,size_t len);
  FILE *fp_;
  char buffer_[64*1024];//setbuffer改变文件的默认缓冲区

public:
  explicit AppendFile(string filename);
  ~AppendFile();

  void append(const char*logline,const size_t len);
  void flush();

};


#endif