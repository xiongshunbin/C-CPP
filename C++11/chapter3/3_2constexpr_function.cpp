#include <iostream>
using namespace std;

/**
 * 为了提高C++程序的执行效率，可以将程序中值不需要发生变化的变量定义为常量，也可以使用constexpr修饰函数的返回值，这种函数称为常量表达式函数。这些函数主要包括：普通函数/类成员函数、类的构造函数、模板函数。
 * 
 * constexpr并不能修饰任意函数的返回值，修饰这些函数成为常量表达式函数，必须满足一下条件：
 *      1.函数必须要有返回值，并且return返回的表达式必须是常量表达式。
 *      2.函数在使用之前，必须有对应的定义语句。
 *      3.整个函数的函数体中，不能出现非常量表达式之外的语句(using指令，typedef语句以及static_assert断言，return语句除外),只能有常量表达式语句。
 * 
 * C++11语法中，constexpr可以修饰模板函数，但由于模板中类型的不确定性，因此模板函数实例化后的函数是否符合常量表达式函数的要求也是不确定的。如果constexpr修饰的模板函数实例化结果不满足常量表达式函数的要求，则constexpr会被自动忽略，即该函数就等同于一个普通函数。
 * 
 * 模板函数是函数模板的实例化。
 * 
 * 如果想要直接得到一个常量对象，也可以使用constexpr修饰一个构造函数，这样就可以得到一个常量构造函数了。常量构造函数有一个要求：构造函数的函数体必须为空，并且必须采用初始化初始化列表的方式为各个成员赋值。
 * 
**/

/*
constexpr void func1()
{
    int a = 100;
    cout << "a: " << a << endl;
}

constexpr int func1()
{
    int a = 100;
    return a;
}
*/

// constexpr int func2();

/*
constexpr int func3()
{
    constexpr int a = 100;
    constexpr int b = 10;
    for(int i = 0; i < b; i ++)  // 常量表达式函数内部不允许出现非常量表达式以外的操作
    {
        cout << "i: " << i << endl;
    }
    return a + b;
}
*/


constexpr int func4()
{
    using mytype = int;
    constexpr mytype a = 100;
    constexpr mytype b = 10;
    constexpr mytype c = a * b;
    return c - (a + b);
}

class Test
{
public:
    constexpr int func()
    {
        constexpr int var = 100;
        return 5 * var;
    }
};

struct Person
{
    const char* name;
    int age;
};

template<typename T>
constexpr T display(T t)
{
    return t;
}

struct T
{
    constexpr T() : a(100)
    {

    }
    int a;
};


int main()
{
    // constexpr int num = func1();
    cout << func4() << endl;
    Test t;
    constexpr int num = t.func();
    cout << "num: " << num << endl;

    struct Person p {"Luffy", 19};
    struct Person ret = display(p);
    // 普通函数
    cout << "Luffy's name: " << ret.name << ", age: " << ret.age << endl;

    // 常量表达式
    constexpr int ret1 = display(250);
    cout << ret1 << endl;

    constexpr struct Person p1{"Luffy", 19};
    constexpr struct Person p2 = display(p1);
    cout << "Luffy's name: " << p2.name << ", age: " << p2.age << endl;

    constexpr T tmp;    // tmp是一个通过常量构造函数生成的常量对象，常量对象里的成员也是不能被修改的。
    // tmp.a = 100;
    cout << tmp.a << endl;

    return 0;
}

/*
constexpr int func2()
{
    constexpr int a = 100;
    return a;
}
*/
