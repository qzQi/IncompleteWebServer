#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"

// 目前不支持自己设置参数
int main(int argc, char *argv[]) {
  int threadNum = 4;
//   IO线程的个数
  int port = 80;
  std::string logPath = "./qzQi_Server.log";


  Logger::setLogFileName(logPath);

  EventLoop mainLoop;
  Server myHTTPServer(&mainLoop, threadNum, port);
  myHTTPServer.start();
  mainLoop.loop();
  return 0;
}
