// const_cast.cpp -- using const_cast operator
#include <iostream>

using namespace std;


int main()
{
    int a = 10;
    cout << "a = " << a << endl;

    const int *p1 = &a;
    // *p1 = 20;

    int *p2 = const_cast<int *>(p1);
    *p2 = 20;
    cout << "a = " << a << endl;
    return 0;
}