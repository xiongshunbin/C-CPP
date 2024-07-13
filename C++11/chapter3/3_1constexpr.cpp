#include <iostream>
using namespace std;

/**
 * constexpr用来修饰常量表达式和常量函数
 * 常量表达式：由多个(>=1)常量(值不会改变)组成并且在编译过程中就得到计算结果的表达式。
 * 常量函数：函数的返回值时一个常量
 * 在C++11之前只有const关键字，从功能上来说这个关键字的有效双重语义：变量只读，修饰常量。
 * 
 * C++程序从编写完毕到执行分为四个阶段：预处理、编译、汇编和链接4个阶段，得到可执行程序之后就可以运行了，需要额外强调的是，常量表达式和非常量表达式的计算时机不同，非常量表达式只能在程序运行阶段计算出结果，但是常量表达式的计算往往发生在程序的编译阶段，这可以极大提高程序的执行效率，因为表达式只需要在编译阶段计算一次，节省了每次程序运行时都需要计算一次的时间。
 * 
 * 在C++11中添加了constexpr关键字用来修饰常量表达式，用来提高程序的执行效率，在使用中建议将const和constexpr的功能区分开。即凡是表达“只读”语义的场景都使用const，表达“常量”语义的场景都使用constexpr。
 * 
 * 在定义常量时，const和constexpr是等价的，都可以在程序的编译阶段计算出结果。
**/

int func(const int num)
{
    int a1 = 520;
    int a2 = 250;
    const int count = 24;   // count是常量
    int array[count];
    int array1[a1];         // 静态数组的定义，数组的大小只能指定为常量
    int array2[num];        // error, num是只读的变量，不可用作常量来用,在函数体内部不能修改他的值。

    int& x = a1;
    const int& y = a1;

    cout << "y: " << y << endl;
    return count;
}

// 对于C++内置类型的数据，可以直接使用constexpr修饰，但如果是自定义的数据类型（用struct或者class实现），直接用constexpr修饰类/结构体定义是不行的。但可以用来修饰类/结构体对象得到常量对象。

/*
constexpr struct Test
{
    int id;
    int num;
};
*/

struct T
{
    int a;
};

int main(int argc, char* argv[])
{
    func(12);
    const int m = func(5);  // 不是常量表达式，m的值只有在运行时才会获取。
    const int i = 520;      // 是一个常量表达式
    const int j = i + 1;    // 是一个常量表达式

    constexpr int x = 520;      // 是一个常量表达式
    constexpr int y = x + 1;    // 是一个常量表达式

    constexpr T t{100};     // 初始化列表可以对任意类型的对象直接进行初始化。

    return 0;
}