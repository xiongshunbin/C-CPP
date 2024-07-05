#include <iostream>
#include <stdexcept>
using namespace std;

// 跟sizeof一样都可以添加参数，noexcept是C++11新添加的关键字或者叫做操作符
// 一般把noexcept放在函数的尾部，表示当前这个函数是不被抛异常的。

class MyException : public logic_error
{
public:
    MyException(const string &what_arg) : logic_error(what_arg) {}
};

// noexcept()可加一个常量表达式返回一个bool值，true: 不会抛异常，false: 会抛异常,即可抛出任意类型的异常。参数的作用相当于动态的开关来控制noexcept的作用，常用于模板。
// throw() 不抛出任何异常 = noexcept = noexcept(true)
void func() noexcept(true)
{
#if 0
    try
    {
        // throw 1;
        throw MyException("string exception!");
        cout << "hello ";
    }
    catch (int e)
    {
        cout << "throw value: " << e << endl;
    }
    catch (MyException e)
    {
        cout << "MyException: " << e.what() << endl;
    }

    cout << "world" << endl;
#endif

    throw MyException("string exception!");
    cout << "good morning!" << endl;
}

int main()
{
    try
    {
        func();
    }
    catch (MyException &e)
    {
        cout << "hello world!" << endl;
    }

    return 0;
}