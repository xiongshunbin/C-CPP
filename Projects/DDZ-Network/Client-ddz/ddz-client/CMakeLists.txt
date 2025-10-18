cmake_minimum_required(VERSION 3.16)

project(ddz-client VERSION 0.1 LANGUAGES CXX)

# 自动处理.ui文件, 生成一个ui_xxx.h文件
set(CMAKE_AUTOUIC ON)
# 自动处理元对象相关的宏, eg: Q_OBJECT
set(CMAKE_AUTOMOC ON)
# 自动处理.qrc文件, 生成相应的的C++代码, 最后链接到可执行程序中
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
# 让编译器严格按照上面指定的标准进行程序的构建, 如果编译器不支持上面的标准, CMake会报错并停止构建
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 搜索Qt相关的组件(Qt 模块)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
)

# 添加头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/card
    ${PROJECT_SOURCE_DIR}/control
    ${PROJECT_SOURCE_DIR}/player
    ${PROJECT_SOURCE_DIR}/strategy
    ${PROJECT_SOURCE_DIR}/thread
    ${PROJECT_SOURCE_DIR}/windows
)

# 添加子目录
add_subdirectory(card)
add_subdirectory(control)
add_subdirectory(player)
add_subdirectory(strategy)
add_subdirectory(thread)
add_subdirectory(windows)

# 链接子目录中生成的静态库文件
link_libraries(windows player thread strategy card control)

# 添加资源文件
qt5_add_resources(RES res.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ddz-client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ddz-client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ddz-client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ddz-client
            ${PROJECT_SOURCES}
            ${RES}
        )
    endif()
endif()

target_link_libraries(ddz-client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ddz-client)
endif()
set_target_properties(ddz-client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ddz-client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ddz-client)
endif()
