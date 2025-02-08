#include <iostream>
#include <string>
using namespace std;

/**
 * C++11中提供的继承构造函数可以让派生类直接使用基类的构造函数，而无需自己在写构造函数，尤其是在基类有很多构造函数的情况下，可以极大的简化派生类构造函数的编写。
 **/

class Base
{
public:
    Base(int i) : m_i(i) {}
    Base(int i, double j) : m_i(i), m_j(j) {}
    Base(int i, double j, string k) : m_i(i), m_j(j), m_k(k) {}
    void print()
    {
        cout << "int: " << m_i << ", double: " << m_j << ", string: " << m_k << endl;
    }

    void func(int i)
    {
        cout << "base class: i = " << i << endl;
    }

    void func(int i, string str)
    {
        cout << "bae class: i = " << i << ", str = " << str << endl;
    }

private:
    int m_i;
    double m_j;
    string m_k;
};

class Child : public Base
{
public:
    using Base::Base;   // 继承父类的构造函数
    using Base::func;   // 使用父类中被隐藏的同名函数
    // Child(int i) : Base(i) {}
    // Child(int i, double j) : Base(i, j) {}
    // Child(int i, double j, string k) : Base(i, j, k) {}

    // 子类中有父类中的同名函数时，将隐藏了父类中的方法。
    void func()
    {
        cout << "child class: i am luffy!!!" << endl;
    }
};

int main()
{
    Child c(520, 13.14, "i love you");
    c.print();
    c.func();
    c.func(19);
    c.func(19, "luffy");
    return 0;
}