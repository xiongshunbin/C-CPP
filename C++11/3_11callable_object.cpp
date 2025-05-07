#include <iostream>

/**
 * 可调用对象：
 *      1.是一个函数指针
 *      2.是一个具有operator()成员函数的类对象(仿函数)
 *      3.是一个可被转换为函数指针的类对象
 *      4.是一个类成员函数指针或类成员指针
*/

void func(void)
{
    std::cout << __FUNCTION__ << std::endl;
}

using func_ptr = int(*)(int);

class Bar
{
public:
    int operator()(int a)
    {
        std::cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

class Foo
{
public:
    static int foo_func(int a)
    {
        std::cout << __FUNCTION__  << "(" << a << ") ->: ";
        return a;
    }

    operator func_ptr()
    {
        std::cout << __FUNCTION__ << std::endl;
        return foo_func;
    }
};



int main()
{

    return 0;
}