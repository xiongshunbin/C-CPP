#include <iostream>
// #include <cassert>
using namespace std;

#if 0
char *createArray(int size)
{
    // assert是一个运行时断言，参数可以是常量表达式，也可以是变量表达式，错误信息是编译器提供
    assert(size > 0);

    char *array = new char[size];
    return array;
}
#endif

/**
 *  32位操作系统 sizeof(long) = 4   sizeof(指针) = 4
 *  64位擦做系统 sizeof(long) = 8   sizeof(指针) = 8
 **/

int main()
{
    // char *buf = createArray(10);
    //  静态断言必须是常量表达式,在编译阶段进行的错误检测，可指定报错信息
    static_assert(sizeof(long) == 8, "当前操作系统不是64位操作系统");
    cout << "sizeof(long): " << sizeof(long) << endl;
    cout << "sizeof(char*): " << sizeof(char *) << endl;
    return 0;
}