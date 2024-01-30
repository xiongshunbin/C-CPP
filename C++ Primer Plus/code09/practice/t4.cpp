#include <iostream>
#include "sales.h"
using namespace std;
int main()
{
    using namespace SALES;
    double ar[4]={11.1,22.2,33.3,44.4};
    Sales s1;
    setSales(s1,ar,3);
    showSales(s1);
    Sales s2;
    setSales(s2);
    showSales(s2);
    
    return 0;
}