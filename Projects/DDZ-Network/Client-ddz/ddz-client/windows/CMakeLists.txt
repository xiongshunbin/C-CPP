cmake_minimum_required(VERSION 3.16)

project(windows)

# 取出当前目录下的所有头文件
set(HEADERS
    AnimationWindow.h
    ButtonGroup.h
    CardPanel.h
    CountDown.h
    EndingPanel.h
    GamePanel.h
    Loading.h
    MyButton.h
    ScorePanel.h
)

# 取出当前目录下的所有源文件
set(SRC
    AnimationWindow.cpp
    ButtonGroup.cpp
    CardPanel.cpp
    CountDown.cpp
    EndingPanel.cpp
    GamePanel.cpp
    Loading.cpp
    MyButton.cpp
    ScorePanel.cpp
)

# 将当前目录下的所有源文件制作成静态库
add_library(windows ${SRC} ${HEADERS})

target_link_libraries(windows PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)
