#include <iostream>
#include <functional>
using namespace std;

void func(int x, int y)
{
    int a = 7;
    int b = 9;
    // 匿名函数的调用方式是：在函数的定义后加入参数列表进行调用。
    // 使用lambda表达式捕获列表捕获外部变量，如果希望去修改按值捕获的外部变量，那么应该使用mutable选项，被mutable修饰的lambda表达式就算没有参数也要写明参数列表，并且可以去掉按值捕获的外部变量的只读（const）属性。
    [=, &x](int z) mutable -> int
    {
        int c = a;
        int d = x;
        b++;
        cout << "b: " << b << endl; // 修改的拷贝值。
        return c + 10;
    }(88);

    // 一般情况下，不指定lambda表达式的返回值，编译器会根据return语句自动推导返回值的类型，但需要注意的是初始化列表不能用于返回值的自动推导,因为初始化列表能初始化的类型有很多。
    /*
    auto f = []()
    {
        return {1, 2};      // error
    }
    */
    cout << "b: " << b << endl;
}

int main()
{
    func(1, 2);
    return 0;
}