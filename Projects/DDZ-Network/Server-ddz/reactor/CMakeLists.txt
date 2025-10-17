cmake_minimum_required(VERSION 3.22)
project(reactor)

set(CMAKE_CXX_STANDARD 20)

# 取出当前目录中所有的源文件, 存储到一个变量中
set(SRC
        Dispatcher.cpp
        EventLoop.cpp
        SelectDispatcher.cpp
        PollDispatcher.cpp
        EpollDispatcher.cpp
)

# 把源文件制作成静态库
add_library(reactor STATIC ${SRC})
