#include <iostream>
#include <map>
using namespace std;

/**
 * auto的应用场景：
 *      1. 用于STL的容器遍历。
 *      2. 用于泛型编程。auto不能推导出模板类型，但可以用于泛型。在使用模板的时候，很多情况下我们不知道变量应该定义为什么类型。
 **/

class T1
{
public:
    static int get()
    {
        return 10;
    }
};

class T2
{
public:
    static string get()
    {
        return "hello world";
    }
};

// template <class T>
template <class T, typename P>
void func(void)
{
    // auto ret = T::get(); // 此时不知道T::get()的类型，可以使用auto
    P ret = T::get();
    cout << "ret: " << ret << endl;
}

int main()
{
    // key：int, value: string
    map<int, string> mp;
    mp.insert(make_pair(1, "ace"));
    mp.insert(make_pair(2, "sabo"));
    mp.insert(make_pair(3, "luffy"));
    // map<int, string>::iterator it = mp.begin();
    auto it = mp.begin();
    for (; it != mp.end(); ++it)
    {
        cout << "key: " << it->first << ", value: " << it->second << endl;
    }

    func<T1, int>();
    func<T2, string>();

    return 0;
}