#include <iostream>
using namespace std;

// 返回值类型后置

/*
// R->返回值类型，T->参数1类型，U->参数2类型
template<typename R, typename T, typename U>
R add(T t, U u)
{
    return t + u;
}
*/

// auto func(argument1,argument2,...) -> decltype(expression)

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u)
{
    return t + u;
}

int main()
{
    int x = 520;
    double y = 13.14;

    // auto ret = add<decltype(x + y), int, double>(x, y);
    auto ret = add<int, double>(x, y);
    auto ret1 = add(x, y);
    cout << "ret: " << ret << endl;

    return 0;
}