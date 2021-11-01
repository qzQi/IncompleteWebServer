set(LIB_SRC
    # all.cc file编译为库文件
    CountDownLatch.cc
    Thread.cc
    ThreadPool.cc
    FileUtil.cc
    LogFile.cc
    Logging.cc
    LogStream.cc
    AsyncLogging.cc

)

add_library(libserver_base ${LIB_SRC})
target_link_libraries(libserver_base pthread rt)

add_subdirectory(test)
# 测试程序。