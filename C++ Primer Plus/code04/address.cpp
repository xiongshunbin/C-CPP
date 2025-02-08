// address.cpp -- using the & operartor to find address
#include<iostream>
int main()
{
	using namespace std;
	
	int donuts=6;
	double cups=4.5;
	
	cout<<"donuts value = "<<donuts;
	cout<<" and donuts address = "<<&donuts<<endl;
	// NOTE: you may need to use unsignd (&donuts)
	// and unsigned (&cups)
	cout<<"cups value = "<<cups;
	cout<<" and cups adress = "<<&cups<<endl;
	
	return 0;
}
