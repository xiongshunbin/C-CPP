#include <iostream>
#include "sales.h"
using namespace std;
int main()
{
	using namespace SALES;
	double ar[4]={11.1,22.2,33.3,44.4};
	Sales s1;
	Sales s2(ar,QUARTERS);

	cout<<"Now show the information of two sales: "<<endl;
	s1.showSales();
	s2.showSales();

	return 0;
}
