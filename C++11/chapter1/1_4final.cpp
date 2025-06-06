#include <iostream>

using namespace std;

/**
 * C++中增加了final关键字，来限制某个类不能被继承，或者某个函虚数不能被重写
 * override关键字确保在派生类中声明的重写函数与基类的虚函数有相同的签名，同时也明确表明将会重写基类的虚函数
 * 多态发生的条件：
 *      1. 有继承关系
 *      2. 子类重写父类虚函数
 *      3. 父类指针或引用指向子类对象
**/ 

class Base
{
public:
    virtual void test() // 一般不会在父类的虚函数的后面加上final
    {
        cout << "Base class...";
    }
};

// 使用final关键字修饰过的类是不允许被继承的，也就是说这个类不能有派生类

class Child final: public Base
{
public:
    void test() final   // 在继承了父类的子类的虚函数后面加上finnal修饰，不让当前子类的子类再重写虚函数
    {
        cout << "Child class...";
    }

/* final修饰函数只能修饰虚函数，不允许修饰普通函数，并且要把final关键字放到类或者函数的后面
    void hello() final
    {

    }
*/
};

// override 只能修饰虚函数，不能用于普通函数，也就是只能用于重写而不是覆盖，并且要把override关键字放在虚函数的后面。

/*
// 非法的
class GrandChild : public Child
{
public:
    // 非法的
    void test()
    {
        cout << "GrandChild class...";
    }
};
*/

int main()
{

    return 0;
}