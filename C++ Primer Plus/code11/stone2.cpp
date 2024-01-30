// stone2.cpp -- user-defined conversion functions
// compile with stonewt1.cpp
#include <iostream>
#include "stonewt2.h"

int main()
{
	using std::cout;
	Stonewt jennySt(9,12);
	double pennyD=146.0;
	Stonewt total;
	total=pennyD+jennySt;
	cout<<"total = ";
	total.show_lbs();
	return 0;
}
