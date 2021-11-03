set(LIB_SRC

    Channel.cc
    Epoll.cc
    EventLoop.cc
    EventLoopThread.cc
    EventLoopThreadPool.cc
    HttpData.cc
    Server.cc
    Timer.cc
    Util.cc
)


include_directories(${PROJECT_SOURCE_DIR}/base)

add_library(libserver_http ${LIB_SRC})
target_link_libraries(libserver_http libserver_base pthread rt)

add_executable(MyServer myserver.cc)
target_link_libraries(MyServer libserver_http)

add_subdirectory(base)
