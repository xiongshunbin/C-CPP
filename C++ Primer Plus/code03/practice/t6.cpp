#include<iostream>
int main()
{
	using namespace std;
	
	double distance_mile,fuel_gallon;
	double distance_kilometer,fuel_litre;
	double fuel_consume;
	cout<<"Enter the distance in miles: ";
	cin>>distance_mile;
	cout<<"Enter the fuel consume in gallon: ";
	cin>>fuel_gallon;
	fuel_consume=distance_mile/fuel_gallon;
	cout<<"The fuel consume is "<<fuel_consume<<" miles/gallon.\n";
	
	cout<<"Enter the distance in kilometer: ";
	cin>>distance_kilometer;
	cout<<"Enter the fuel consume in litre: ";
	cin>>fuel_litre;
	fuel_consume=100*(fuel_litre/distance_kilometer);
	cout<<"The fuel consume is "<<fuel_consume<<" L/100km.\n";
	
	return 0;
}
