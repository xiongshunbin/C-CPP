// staticcast.cpp -- using static_cast<> in class's inheritance
#include <iostream>

using namespace std;

class A
{

};

class B : public A
{

};

class C
{

};

int main()
{
    A a;
    B b;
    A *pa = static_cast<A *>(&b);
    B *pb = static_cast<B *>(&a);

    // C *pc = static_cast<C *>(&a);
    return 0;
}