#include <iostream>
#include <string>
#include <map>
using namespace std;

/**
 * 当默认模板参数和模板参数自动推导同时使用时（优先级从高到低）：
 *      1. 如果显示的指定了模板类型则使用指定的模板类型
 *      2. 如果可以推导出参数类型则使用推导出的类型（指定实参）
 *      3. 如果函数模板无法推导出参数类型，那么编译器会使用默认模板参数
 *      4. 如果无法推导出模板参数类型并且没有设置默认模板参数，编译器会报错
 * 模板类<>不可省略，而模板函数可以省略（函数模板若提供了默认模板参数，或者通过实参进行模板参数自动推导的情况下）
 **/

// 注意：非类型参数和类型参数的区别。可以为非类型参数提供默认值，对于类模板和函数模板都是适用的。同样的，为模板类型参数提供默认值，对于类和函数也都适用。

template <typename T>
class Container
{
public:
    void print(T &t)
    {
        auto it = t.begin();
        for (; it != t.end(); ++it)
        {
            cout << it->first << ", " << it->second << endl;
        }
    }
};

// 默认的模板类型参数不能用于推导
template <typename T = long, typename U = int>
void mytest(T t = 'A', U u = 'B')
{
    cout << "t: " << t << ", u: " << u << endl;
}

int main(int argc, char *argv[])
{
    // 根据传递的实参进行自动类型推导
    mytest('a', 'b');            // mytest<char, char>
    mytest<int>('a', 'b');       // mytest<int, char>
    mytest<char>('a', 'b');      // mytest<char, char>
    mytest<int, char>('a', 'b'); // mytest<int, char>
    mytest<char, int>('a', 'b'); // mytest<char, int>
    mytest();                    // mytext<long, int>

    map<int, string> m;
    m.insert(make_pair(1, "ace"));
    m.insert(make_pair(2, "sabo"));
    m.insert(make_pair(3, "luffy"));
    Container<map<int, string>> c;
    c.print(m);

    return 0;
}
