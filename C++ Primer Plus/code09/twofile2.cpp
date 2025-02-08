// twofile2.cpp -- variable with internal and external linkage
#include<iostream>	
extern int tom;			// tom defined elsewhere	
static int dick;		// overrides external dick
int harry=200;			// external variable definition,
						// no conflict with twofile1 harry
void remote_access()
{
	static int dick=0.5; 
	using namespace std;
	cout<<"remote_access() reports the following address:\n";
	cout<<&tom<<" = &tom, "<<&dick<<" = &dick, ";
	cout<<&harry<<" = &harry\n";
	cout<<"&::dick = "<<&(::dick);
}
