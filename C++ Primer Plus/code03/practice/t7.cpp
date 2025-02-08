#include<iostream>
int main()
{
	using namespace std;
	
	double temp,fuel_consume;
	cout<<"Enter the fuel consume(L/100km):";
	cin>>temp;
	fuel_consume=62.14*3.785/temp;
	cout<<temp<<" L/100km = "<<fuel_consume<<" mpg.";
	
	return 0;
}
