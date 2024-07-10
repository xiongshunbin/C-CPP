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

// 函数声明
int func_int() {}
int &func_int_r() {}
int &&func_int_rr() {}
const int func_cint() {}
const int &func_cint_r() {}
const int &&func_cint_rr() {}
const Test func_ctest() {}

// 如果函数返回的是一个纯右值且不属于任何的类（字面量，固定的值，在表达式执行结束后不再存在的数据，也就是临时性的数据），对于纯右值而言，只有类类型可以携带const、valatile限定符，除此之外需要忽略掉这两个限定符。

int main()
{
    int n = 100;
    // a : int
    decltype(func_int()) a = 0;
    // b : int&
    decltype(func_int_r()) b = n;
    // c : int&&
    decltype(func_int_rr()) c = 0;
    // d : int
    decltype(func_cint()) d = 0;
    // e : const int&
    decltype(func_cint_r()) e = 0;
    // f : const int&&
    decltype(func_cint_rr()) f = 0;
    // g : const Test
    decltype(func_ctest()) g = Test();

    return 0;
}