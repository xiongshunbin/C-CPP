#include <iostream>
#include "move.h"

int main()
{
	double x,y;
	Move m1(11.1,22.2);
	Move m2(10.0,20.0);
	Move m3;
	cout<<"m1: ";
	m1.showmove();
	cout<<"m2: ";
	m2.showmove();

	cout<<"Enter new x and y: ";
	cin>>x>>y;
	cout<<"Reset m1 and show:"<<endl;
	m1.reset(x,y);
	m1.showmove();

	m3=m1.add(m2);
	cout<<"Show the add of m1 and m2: "<<endl;
	cout<<"m3: ";
	m3.showmove();

	return 0;
}

