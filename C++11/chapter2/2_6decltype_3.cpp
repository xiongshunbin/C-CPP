#include <iostream>
using namespace std;

// decltype(expression)  不需要进行初始化，不会对expression进行运算，基于expression推导出数据类型来定义一个新的变量。

/**
 * 推到规则：
 *      1.表达式为普通变量或者普通表达式或者类表达式，在这种情况下，使用decltype推导出的类型和表达式的类型是一致的。
 *      2.表达式是函数调用，使用decltype推导出的类型和函数返回值一致。
 *      3.表达式是一个左值(可以取地址的变量)，或者被括号()包围，使用decltype推导出的是表达式类型的引用（如果有const、volatile限定符，不能忽略）
 **/

class Test
{
public:
    int num = 9;
    string text;
    static const int value = 110;
};

int main()
{
    const Test obj;

    // 带有括号的表达式
    // a : int
    decltype(obj.num) a = 0;
    // b : const int&
    decltype((obj.num)) b = a;
    // 加法表达式
    int n = 0, m = 0;
    // c : int
    decltype(n + m) c = 0;
    // d : int&
    decltype(n = n + m) d = n; // 对m进行操作之后的结果还是左值
    decltype(m) xx;            // m是普通变量

    return 0;
}