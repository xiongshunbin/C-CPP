cmake_minimum_required(VERSION 3.22)
project(tcp)

set(CMAKE_CXX_STANDARD 20)

# 取出当前目录中所有的源文件, 存储到一个变量中
set(SRC
        TcpServer.cpp
        TcpConnection.cpp
)

# 把源文件制作成静态库
add_library(tcp STATIC ${SRC})
