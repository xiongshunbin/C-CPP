// stone.cpp -- user-defined conversions
// compile with stonewt.cpp
#include <iostream>
using std::cout;
#include "stonewt.h"
void display(const Stonewt & st,int n);
int main()
{
	Stonewt incognito=275;	// uses constructor to initialize
	Stonewt wolfe(285.7);	// same as Stonewt wolf = 285.7
	Stonewt taft(21,8);
	Stonewt result;

	cout<<incognito;
	cout<<wolfe;
	cout<<taft;

	result=incognito+wolfe;
	result.set_type(Stonewt::STONE);
	cout<<"result = "<<result;

	result=wolfe-incognito;
	cout<<"result = "<<result;

	result=incognito*2;
	result.set_type(Stonewt::POUNDS);
	cout<<"result = "<<result;

	return 0;
}

