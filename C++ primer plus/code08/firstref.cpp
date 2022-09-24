// firstref.cpp -- defining and using a reference
#include<iostream>
int main()
{
	using namespace std;
	int rats=101;
	int & rodents=rats;	// rodents is a reference
	cout<<"rats = "<<rats;
	cout<<", rodents = "<<rodents<<endl;
	rodents++;
	cout<<"rats = "<<rats;
	cout<<", rodents = "<<rodents<<endl;
	
	// some implementations require type casting the following
	// adress to type unsigned
	cout<<"rats adress = "<<&rats;
	cout<<", rodents adress = "<<&rodents<<endl;
	
	return 0;
}
