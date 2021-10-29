set(LIB_SRC
    # all.cc file编译为库文件
    CountDownLatch.cc
    Thread.cc
)

add_library(libserver_base ${LIB_SRC})
target_link_libraries(libserver_base pthread rt)

add_subdirectory(test)
# 测试程序。