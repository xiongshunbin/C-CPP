#include <iostream>
#include "golf.h"
int main()
{
	Golf p1=Golf();
	Golf p2("Ann Birdfree",24);

	cout<<"Now show the handicap of two person: "<<endl;
	p1.showgolf();
	p2.showgolf();

	return 0;
}
