#include <iostream>
#include "plorg.h"

int main()
{
	Plorg p1;
	p1.show();

	cout<<"After reset CI:"<<endl;
	p1.reset(100);
	p1.show();
	return 0;
}
