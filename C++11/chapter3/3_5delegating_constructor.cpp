#include <iostream>
using namespace std;

/**
 * 委托构造函数：允许使用同一个类中的一个构造函数调用其他的构造函数，从而简化相关类成员的初始化，优化代码。
 * 
 * 注意：
 *      1.链式的构造函数调用不能形成一个闭环(死循环)，否则会在运行期抛异常。
 *      2.如果要进行多层构造函数的链式调用，建议将构造函数的调用写在初始化列表中，而不是函数体内部，否则编译器会提示形参的重复定义。
 *      3.在初始化列表中调用了代理构造函数初始化某个类成员变量之后，就不能在初始化列表中再次初始化这个变量了。
 *      4.在初始化中使用了委托构造函数后，也不能在初始化列表中初始化其他类成员。
 **/

class Test
{
public:
    Test(){};
    Test(int max)
    {
        this->m_max = max > 0 ? max : 100;
    }

    Test(int max, int min) : Test(max)//, m_max(max)
    {
        // Test(max);
        this->m_min = min > 0 && min < max ? min : 1;
    }

    Test(int max, int min, int mid) : Test(max, min)//, m_mid(mid)
    {
        // Test(max, min);
        this->m_middle = mid < max && mid > min ? mid : 50;
    }

private:
    int m_min;
    int m_max;
    int m_middle;
};

int main()
{
    return 0;
}