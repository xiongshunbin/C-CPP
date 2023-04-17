#include <iostream>
#include "bop.h"
using namespace std;

int main()
{
    Port port1("Gallo", "tawny", 20);
    cout << port1 << endl;

    VintageaPort vp1("Kawen", "vintage", 50, "KW", 40); 
    cout << vp1 << endl;

    VintageaPort vp2 = vp1;
    cout << vp2 << endl;

    Port * p;
    p = &port1;
    p -> Show();

    p = &vp1;
    p -> Show();

    return 0;
}