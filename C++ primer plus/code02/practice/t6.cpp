#include<iostream>

using namespace std;
double convert(double);

int main()
{
	double light_years;
	double astro_units;
	
	cout<<"Enter the number of light years: ";
	cin>>light_years;
	astro_units = convert(light_years);
	cout<<light_years<<" light years = "<<astro_units<<" astronomical units."<<endl;
	return 0;
}
double convert(double years)
{
	return years*63240;
}
