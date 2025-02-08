#include <iostream>
using namespace std;

class Base
{
public:
    Base() : a(250) { }
    Base(int num) : a(num) { }

    int get_a() const
    {
        return a;
    }

private:
    // C++11允许非静态成员变量在类内部进行初始化
    int a{4};
    int b = 1;
    double cc = 1.23;
    char aa = 'a';
    // C++11允许对类内部的对象使用=或{}进行初始化
    string str = "hello";
    string str1{"world"};
    // string str2("hello,world");  // invalid, 在类的内部不能通过调用对象成员的构造函数进行初始化，即不能使用()进行实例化
    // C++11不允许对静态成员变量,静态非int型常量进行就地初始化
    static int c;
    static const double d;
    static const char* const e;
    // C++98 标准：变量是静态常量，并且类型是整型或者枚举类型
    const static int f = 0;
};

int Base::c = 0;
const double Base::d = 3.14;
const char* const Base::e = "i am luffy";

// C++11中在类内部有两种初始化方式：= {}
/**
 * C++11以前，对类成员变量进行初始化有2种方式：
 *  1. 在构造函数函数体内部进行初始化（赋值）
 *  2. 在构造函数对应的初始化列表中进行初始化（√，初始化）
 *  3. 在类内部就地初始化效率最高
 * 在类内部进行了就地初始化，又在初始化列表中进行了初始化，初始化列表中初始化的值会覆盖掉就地初始化的值
**/


int main()
{
    Base b;
    cout << "a value: " << b.get_a() << endl;   // a value: 250
    return 0;
}