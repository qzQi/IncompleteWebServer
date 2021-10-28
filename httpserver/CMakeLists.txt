set(SRCS 
# all the .cc source code

)


include_directories(${PROJECT_SOURCE_DIR}/base)

add_executable(myserver ${SRCS})
target_link_libraries(myserver libserver_base)

add_subdirectory(base)
