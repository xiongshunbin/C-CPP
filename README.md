# C++ 学习

本仓库存放的是我学习C++的过程中所阅读的书籍以及所做过的项目的源码。

## 阅读的书籍

  1. 《C++ Primer Plus》
  2. 《C++ Primer》
  3. 《TCP/IP网络编程》

## 做过的项目

  1. Landlords -- 单机版斗地主项目，该项目采用Qt5.15.2+MSVC构建，代码行数约5000+行，为Qt练手项目,主要涉及：STL的使用，复杂业务流程的设计，信号在不同窗口之间的传递，机器人出牌算法的设计。
  2. IB_System -- 智慧公交系统，该项目使用Qt5.15.2+MinGW构建，主要涉及的技术有通过Qt编译MySQL套件，以及Qt连接MySQL数据库。
  3. TDGame -- 村庄保卫战，该项目使用SDL2游戏引擎，代码行数约4000+行，包含的核心技术有：自定义Timer计时器类的实现，自定义Animation动画类的实现，可继承单例模板类的实现，使用cJson库进行json文件的解析，对csv文件格式的解析。通过本项目的锻炼，增强了我对于项目开发的整体观的把握的深刻理解和意识。

## 其他

    1. Linux 文件夹里存放的是学习Linux系统编程的代码
    2. ObjectOriented 文件夹存放的是23种设计模式的C++实现
    3. C++11 文件夹存放的是常用的C++11新特性
    4. TCP-IP Socket文件夹存放的是学习TCP/IP网络编程一书的代码。
    5. Projetc文件夹存放所有我做过的项目
    6. C++ Primer 和 C++ Primer Plus分别存放C++学习的课本源码和课后习题源码。

## 目录结构

```shell
C-CPP
├── C++ Primer
│   └── chapter01
├── C++ Primer Plus
│   ├── code02
│   ├── code03
│   ├── code04
│   ├── code05
│   ├── code06
│   ├── code07
│   ├── code08
│   ├── code09
│   ├── code10
│   ├── code11
│   ├── code12
│   ├── code13
│   ├── code14
│   ├── code15
│   └── code16
├── C++11
│   ├── 1_1RawString.cpp
│   ├── 1_2longlong.cpp
│   └── C++11新特性.md
├── Linux
│   ├── 10_pipe_wr.c
│   ├── 1_pid.c
│   ├── 2_fork_1.c
│   ├── 3_fork_2.c
│   ├── 4_fork_3.c
│   ├── 5_wait.c
│   ├── 6_thread.c
│   ├── 7_multi_thread.c
│   ├── 8_pipe_1.c
│   └── 9_pipe_size.c
├── ObjectOriented
│   ├── SimpleFactory.cpp
│   └── Singleton.cpp
├── Project
│   ├── IB_System
│   ├── Landlords
│   └── TdGame
└── README.md
```
