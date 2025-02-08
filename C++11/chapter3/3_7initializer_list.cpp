#include <iostream>
using namespace std;

/**
 * 关于C++中的变量，数组，对象等都有不同的初始化方法，在这些繁琐的初始化方法中没有任何一种方式适用于所有的情况。为了统一初始化方式，并且让初始化行为具有确定的效果。在C++11中提出了列表初始化的概念。
 **/

class Person
{
public:
    Person(int num) : m_num(num) {}
    Person(int num, string name) : m_name(name), m_num(num) {}

private:
    Person(const Person &);

private:
    int m_num;
    string m_name;
};

Person getPerson()
{
    return {520, "华安"};   // 相当于return Person(520, "华安");
}

struct T
{
    int a = 5;
    int array[3]{1, 2, 3};
    int c;
};

int main(void)
{
    Person t1(520);
    Person t2 = 520;   // 隐式类型转化,这里只调用了只有一个参数的构造函数
    Person t3 = {520}; // 初始化列表
    Person t4{520};    // 初始化列表

    int a1 = {1314};
    int a2{1314};
    int arr1[] = {1, 2, 3};
    int arr2[]{1, 2, 3};

    // 使用初始化列表也可以初始化new出来的堆内存
    int *p = new int{520};
    int *q = new int(3); // 此时不是使用初始化列表进行初始化
    double b = double{52.134};
    int *array = new int[3]{1, 2, 3};

    return 0;
}