#include<iostream>

using namespace std;
double convert(double);

int main()
{
	double c_degree;
	double f_degree;
	
	cout<<"Please enter a Celsius value: ";
	cin>>c_degree;
	f_degree=convert(c_degree);
	cout<<c_degree<<" degrees Celsius is "<<f_degree<<" degrees Fahrenheit.";
	return 0;
}
double convert(double degree)
{
	return degree*1.8+32.0;
}
