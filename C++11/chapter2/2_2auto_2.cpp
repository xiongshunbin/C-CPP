#include <iostream>
using namespace std;

/**
 * auto关键字并不是万能的，在以下场景中是不能完成类型推导的
 * 1.不能作为函数参数使用。因为只有在函数调用的时候才会给函数参数传递实参，auto要求必须要给修饰的变量赋值，因此二者矛盾。
 * 2.不能用于类的非静态成员变量的初始化
 * 3.不能使用auto关键在定义数组
 * 4.无法使用auto推导出模板参数
 **/

#if 0
int func(auto a, auto b)
{
    cout << "a: " << a << ", b: " << b << endl;
}
#endif

/*
class Test
{
    // auto v1 = 0; // invalid 类的非静态成员不属于类，属于类对象，只有对象被创建出来之后，才能够对变量进行初始化。
    // static auto v2 = 0;  // invalid 类的静态非常量成员不允许在类内部直接初始化
    static const auto v1 = 10; // valid
};
*/

template<typename T>
class Test{ };

int main()
{
    // func(1, 2);
    int array[] = {1, 2, 3, 4, 5}; // 定义数组
    auto t1 = array;               // valid, t1被推导为int*类型
    // auto t2[] = array;              // invalid
    // auto t3[] = {1, 2, 3, 4, 5};    // invalid

    Test<double> t;
    // Test<auto> t1 = t;       // invalid,无法推导出模板类型

    return 0;
}