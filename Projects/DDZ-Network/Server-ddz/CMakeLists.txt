cmake_minimum_required(VERSION 3.22)
project(Server_ddz)

set(CMAKE_CXX_STANDARD 20)

# 添加头文件目录
include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
)

# 添加子目录
add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)

# 链接子目录中生成的静态库文件
link_libraries(tcp http reactor thread common)

# 指定可执行文件生成到对应的目录中
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(Server_ddz main.cpp)

# 链接系统的spdlog库
find_package(spdlog REQUIRED)
target_link_libraries(Server_ddz PRIVATE spdlog::spdlog_header_only)
