// support.cpp -- use external variable
// compile with external.cpp
#include "support.h"

using std::cout;
void update(double dt)
{
	warming+=dt;
	cout<<"Updating global warming to "<<warming;
	cout<<" degrees.\n";
}
void local()
{
	double warming=0.8;
	
	cout<<"Local warming is = "<<warming<<" degrees.\n";
	// Access global variable with the
	// scop resolution operator
	cout<<"But global warming = "<<::warming;
	cout<<" degrees.\n";
}
