#include <iostream>
#include <string>
#include <map>
using namespace std;

/**
 * using的功能：
 *      1.声明命名空间，使用命名空间可以防止命名冲突。
 *      2.派生类函数重载。当父类中有多个重载函数，且子类中有与父类中重载函数的同名函数时，子类将隐藏父类中的同名函数，在通过子类对象调用该函数时，只能调用子类中的方法同名方法。using 父类::函数名; 这样就可以通过子类对象调用被隐藏的父类里的同名函数。
 *      3.定义类型的别名,跟typedef定义的别名在语义上是等效的，被定义的类型并不是一个新的类型，仅仅只是原有的类型取了一个新的名字。
 * 
 * typedef OldName NewName
 * using NewName = oldName
 * 
 * 类型别名和类型的名字等价，只要是类型的名字弧线的地方，就能使用类型别名。
 * using还可给一个模板定义别名，但typedef不行
**/

template<typename T>
class Container
{
public:
    void print(T& t)
    {
        auto it = t.begin();
        for(; it != t.end(); ++ it)
        {
            cout << it->first << ", " << it->second << endl;
        }
    }
};

int mytest(int a, string b)
{
    cout << "a: " << a << ", b: " << b << endl;
    return 0;
}

typedef int (*func)(int, string);
using func1 = int (*)(int, string);

int main()
{
    func f = mytest;
    func1 f1 = mytest;
    f(10, "hello");
    f1(10, "hello");
    (*f)(10, "s");

    typedef int t1;
    t1 a = 10;
    t1 b = 5;

    using t2 = int;
    t2 a1  = 3;

    // map: int-int, int-double, int-string
    map<int,string> m;
    m.insert(make_pair(1, "ace"));
    m.insert(make_pair(2, "sabo"));
    m.insert(make_pair(3, "luffy"));
    Container<map<int, string>> c;
    c.print(m);
    return 0;
}