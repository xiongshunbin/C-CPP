cmake_minimum_required(VERSION 3.16)

project(thread)

# 取出当前目录下的所有头文件
set(HEADERS
    RobotGrabLord.h
    RobotPlayHand.h
)

# 取出当前目录下的所有源文件
set(SRC
    RobotGrabLord.cpp
    RobotPlayHand.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(thread ${SRC} ${HEADERS})

target_link_libraries(thread PRIVATE Qt${QT_VERSION_MAJOR}::Core)
