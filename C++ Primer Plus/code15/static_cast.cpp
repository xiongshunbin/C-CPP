// static_cast.cpp - using static_cast
#include <iostream>

using namespace std;

int main()
{
    // double x = (double)10 / 3;  // C style
    double x = 10 / 3;
    cout << "x = " << x << endl;

    double y = static_cast<double>(10) / 3;
    cout << "y = " << y << endl;
    return 0;
}