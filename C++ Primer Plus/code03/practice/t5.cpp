#include<iostream>
int main()
{
	using namespace std;
	long long World,America;
	cout<<"Enter the world's population: ";
	cin>>World;
	cout<<"Enter the population of the US: ";
	cin>>America;
	cout<<"The population of the US is "
		<<(double)America/World*100<<"% of the world population.";
	
	return 0;
}

