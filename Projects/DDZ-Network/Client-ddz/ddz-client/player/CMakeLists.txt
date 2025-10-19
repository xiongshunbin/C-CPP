cmake_minimum_required(VERSION 3.16)

project(player)

# 取出当前目录下的所有头文件
set(HEADERS
    Player.h
    Robot.h
    UserPlayer.h
)

# 取出当前目录下的所有源文件
set(SRC
    Player.cpp
    Robot.cpp
    UserPlayer.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(player ${SRC} ${HEADERS})

target_link_libraries(player PRIVATE Qt${QT_VERSION_MAJOR}::Core)
