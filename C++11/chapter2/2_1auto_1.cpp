#include <iostream>

using namespace std;

/**
 * C++11自动类型推导有两种：auto关键字和decltype关键字
 * 使用auto时，必须要对变量进行初始化，才能够推导出变量对应的类型
 * 使用decltype时，是基于一个已经存在的变量，去推导对应的类型，用于定义新的变量
 * 
 * auto 变量名 = 变量值
 * 当变量不是指针或者引用类型时，推导的结果中不会保留const、volatile关键字
 * 当变量是指针或者引用类型时，推导的结果中会保留const、volatile关键字
 * 
 * 注：const修饰变量，指明这个变量是只读的或者修饰这个变量是一个常量
 *    volatile修饰变量，指明他就是一个变量，而且是一个经常变化的值，告诉编译器在处理该变量时不要进行任何的优化，一般用于多线程编程。
 * 
 * 注意 const int* 和 int* const的区别：
 *      1. const int*： 常量指针，不能修改指针所指向地址的值
 *      2. int* const: 指针类型的常量，不能修改指针指向的地址
 **/

int main()
{
    // 没有 const 修饰
    // double
    auto x = 3.14;
    // int
    auto y = 520;
    // char
    auto z = 'a';
    // auto nb;             // invalid
    // auto double nb1;     // invalid, 不能修改数据类型

    int temp = 110;
    // &temp: int*
    // auto *a = int*
    // auto : int
    auto *a = &temp;
    // &temp: int*
    // auto : int*
    auto b = &temp;
    // int
    auto &c = temp;
    // int
    auto d = temp;

    // 有const修饰
    int tmp = 250;
    // a1 : const int
    const auto a1 = temp;
    // a1 : const int
    // a2 : const int
    // a2 : int
    auto a2 = a1;
    // a3 : conat int&
    // a4 : const int&
    const auto &a3 = temp;
    auto &a4 = a3;
    // const int*
    auto *pt4 = &a1;
    // *pt4 = 400;

    return 0;
}