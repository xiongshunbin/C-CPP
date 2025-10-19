cmake_minimum_required(VERSION 3.16)

project(card)

# 取出当前目录下的所有头文件
set(HEADERS
    Card.h
    Cards.h
)

# 取出当前目录下的所有源文件
set(SRC
    Card.cpp
    Cards.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(card ${SRC} ${HEADERS})

target_link_libraries(card PRIVATE Qt${QT_VERSION_MAJOR}::Core)
