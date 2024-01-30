// stone3.cpp -- user-defined conversions
// compile with stonewt.cpp
#include <iostream>
using std::cout;
#include "stonewt3.h"
int main()
{
	Stonewt taft(10,8);
	Stonewt result=taft*2;

	result.show_lbs();
	result.show_stn();

	Stonewt myCat;
	myCat=2*taft;
	myCat.show_lbs();
	myCat.show_stn();
	return 0;
}

