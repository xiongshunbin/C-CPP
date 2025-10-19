cmake_minimum_required(VERSION 3.16)

project(control)

# 取出当前目录下的所有头文件
set(HEADERS
    BGMControl.h
    GameControl.h
)

# 取出当前目录下的所有源文件
set(SRC
    BGMControl.cpp
    GameControl.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(control ${SRC} ${HEADERS})

target_link_libraries(control PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
)
