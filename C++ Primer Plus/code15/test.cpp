// test.cpp -- Comparing the differences between virtual functions and functions with the same name
#include <iostream>

using namespace std;

class Temp
{
private:
    int x, y;
public:
    Temp(int a, int b) : x(a), y (b) { }
    virtual ~Temp() { }
    #if 0
    virtual void show()
    {
        cout << "x = " << x << ", y = " << y << endl;
    }
    #else
    void show()
    {
        cout << "x = " << x << ", y = " << y << endl;
    }
    #endif
};

class A : public Temp
{
private:
    string info;
public:
    A(int a, int b, string message) : Temp(a, b), info(message) { }
    virtual ~A() { }
    void show()
    {
        cout << info << endl;
        Temp::show();
    }
};

class B : public Temp
{
private:
    double rate;
public:
    B(int a, int b, double r) : Temp(a, b), rate(r) { }
    virtual ~B() { }
    void show(double add)       //  base and derived class have the same name functions
    {
        cout << "rate: " << rate + add << endl;
        Temp::show();
    }
};

int main()
{
    Temp* t1 = new Temp(1, 2);
    // Temp* t2 = new A(3, 4, "This is A object!");
    // A a(5, 6, "This is A object");
    t1->show();
    // t2->show();
    // a.show();
    // a.Temp::show();

    // Temp* t3 = new B(7, 8, 3.14);
    B b(9, 10, 3.14);
    // t3->show();
    // b.show(2.1);
    b.Temp::show();
    return 0;
}