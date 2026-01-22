#include <iostream>
#include <thread>

/**
 * 尽量避免在传递给线程的函数中使用重载，因为这会增加编译器解析的复杂性，并且很可能解析失败。
 */
void threadFunction(int& some_param);
// void threadFunction(int* some_param);

/**
 * 当子线程中需要修改主线程中的局部变量时，可向线程函数传递指针或引用std::ref()参数。
 * 
 * 在向线程函数传递参数时，直接向 std::thread 的构造函数中增添参数即可。
 * 过程:
 *      1.将参数以默认方式(按值传递)先复制到线程对象的内部存储空间形成副本
 *      2.将参数的副本(move-only)以右值形式传给新线程上的函数或可调用对象
 */

int main()
{
    int param = 0;

    std::thread t1(threadFunction, std::ref(param));
    t1.join();

    std::cout << "param = " << param << std::endl;

    return 0;
}

void threadFunction(int& some_param)
{
    some_param += 100;
}

/*
void threadFunction(int* some_param)
{
    *some_param += 100;
}
*/
