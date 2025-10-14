cmake_minimum_required(VERSION 3.15)
project(LoggerTest)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utility)

add_subdirectory(utility)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/utility)

add_executable(Test main.cpp)
target_link_libraries(Test )