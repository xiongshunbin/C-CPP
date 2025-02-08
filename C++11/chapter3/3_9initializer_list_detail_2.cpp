#include <iostream>
using namespace std;

/**
 * 使用初始化列表对非聚合类型的对象：
 *      1.有用户自定义的构造函数。
 *      2.有私有或保护的非静态数据据成员
 *      3.有基类
 *      4.有虚函数
 * 给非聚合类型的类提供一个对应的构造函数，就可以使用初始化列表为这个类对象进行初始化。
 * 聚合类型的定义并非递归的，也就是说当一个类的非静态成员是非聚合类型时，这个类也可能是聚合类型
 **/

class Person
{
public:
    Person(int num) : m_num(num) {}
    Person(int num, string name) : m_name(name), m_num(num) {}

private:
    int m_num;
    string m_name;
};

Person getPerson()
{
    return {520, "华安"};
}

// T1是非聚合类型
class T1
{
    int x;
    double y;

private:
    int z;
};

// T2是聚合类型
struct T2
{
    T1 t1;
    long x1;
    double y1;
};

int main()
{
    Person p{9527, "华安"};
    T2 t2{{}, 520, 13.14};
    return 0;
}