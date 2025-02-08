// external.cpp -- external variables
// compile with support.cpp
#include<iostream>
#include "support.h"

using namespace std;
// external variable
double warming=0.3;	// warming defined
// function prototypes
void update(double dt);
void local();

int main()
{
	cout<<"Globle warming is "<<warming<<" degrees.\n";
	update(0.1);	// call function to change warming
	cout<<"Globle warming is "<<warming<<" degrees.\n";
	local();
	cout<<"Globle warming is "<<warming<<" degrees.\n";
	
	return 0;
}

