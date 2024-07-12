#include <iostream>
#include <functional>
using namespace std;

/**
 * 使用lambda表达式捕获列表捕获外部变量，如果希望去修改按值捕获的外部变量，那么应该使用mutable选项，被mutable修饰的lambda表达式就算没有参数也要写明参数列表，并且可以去掉按值捕获的外部变量的只读（const）属性。
 * lambda表达式也属于可调用对象，可调用对象的种类有：函数指针，仿函数，可以转换为函数指针的对象，类中的成员函数和成员变量。
 * 为什么通过值拷贝的方式捕获的外部变量是只读的？
 *      1.lambda表达式的类型在C++11中会被看作是一个带operator()的类，即仿函数
 *      2.按照C++标准，lambda表达式的operator()默认是const的，一个const成员函数是无法修改成员变量值的。
 *
 * mutable选项的作用就在于取消operator()的const属性。
 *
 * 仿函数：重载了operator()的类，类对象都可以作为函数进行调用。
 *
 * 因为lambda表达式在C++中会被看成是一个仿函数，因此可以使用std::function和std::bind来存储和操作lambda表达式。
 *
 * 在什么情况下，匿名函数可以作为函数指针来使用？
 *      1. 对于没有捕获任何变量的lambda表达式，还可以转换成一个普通的函数指针。
 *      2. 如果捕获列表中捕获了外部变量，匿名函数只能作为仿函数来使用。
 *
 * 把匿名函数对应的仿函数保存起来的方式：
 *      1. 使用可调用对象包装器function()
 *      2. 使用可调用对象绑定器bind()
 **/

void func(int x, int y)
{
    int a;
    int b;
    using ptr = void (*)(int);

    function<void(int)> fff = [](int x)
    {
        cout << "x: " << x << endl;
    };
    fff(11);

    // 对匿名函数进行绑定，匿名函数的函数体就可以作为函数的地址。
    // bind的第一个参数为lambda表达式的函数体，第二个参数为指定绑定的匿名函数的参数列表
    function<void(int)> f1 = bind([](int x) // 包装器对象
    {
        cout << "x: " << x << endl;
    }, placeholders::_1);

    auto f2 = bind([](int x)    // 仿函数
    {
        cout << "x: " << x << endl;
    }, placeholders::_1);

    /**
     * 绑定完成之后得到的就是一个仿函数，可以保存到包装器对象中，也可以使用auto来进行自动类型推导，包装器类型可以包装auto推导出来的类型。两者的区别：
     * 使用包装器进行包装，包装之前匿名函数有几个参数，包装之后，得到的可调用对象就有几个参数。使用可调用对象绑定器进行绑定时，包装之前匿名函数如果有N个参数，包装之后得到的仿函数的参数可 <= N,绑定器函数可以进行降元操作。
    **/

    /*
    ptr p2 = [=](int x)     // 此时，lambda表达式不能被看作是函数指针，而只能被看作是仿函数。
    {
        cout << "x: " << x << endl;
    };
    p1(11);
    */
}

int main()
{
    using func_ptr = int (*)(int);
    // 没有捕获任何外部变量的函数
    func_ptr f = [](int a)
    {
        return a;
    };

    return 0;
}