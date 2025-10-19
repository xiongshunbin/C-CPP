cmake_minimum_required(VERSION 3.16)

project(strategy)

# 取出当前目录下的所有头文件
set(HEADERS
    PlayHand.h
    Strategy.h
)

# 取出当前目录下的所有源文件
set(SRC
    PlayHand.cpp
    Strategy.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(strategy ${SRC} ${HEADERS})

target_link_libraries(strategy PRIVATE Qt${QT_VERSION_MAJOR}::Core)
