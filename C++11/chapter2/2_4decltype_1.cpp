#include <iostream>
using namespace std;

class Test
{
public:
    int num = 9;
    string text;
    static const int value = 110;
};

int main()
{
    int x = 99;
    const int &y = x;
    // a : int
    decltype(x) a = x;
    // y : const int&
    // b : const int &
    decltype(y) b = x;
    // const int
    decltype(Test::value) c = 0;
    Test t;
    // string
    decltype(t.text) d = "hello, world";
    return 0;
}