#include<iostream>
using namespace std;
struct Pizza
{
	char company[20];
	float diameter;
	float weight;
};
int main()
{
	Pizza dinner;
	cout<<"Enter the pizza's company:"<<endl;
	cin.getline(dinner.company,20);
	cout<<"Enter the size of pizza in inches:"<<endl;
	cin>>dinner.diameter;
	cout<<"Enter the weight of pizza in pounds:"<<endl;
	cin>>dinner.weight;
	
	cout<<"Pizza'company:"<<dinner.company<<", "
		<<"size: "<<dinner.diameter<<", "
		<<"weight: "<<dinner.weight<<"."<<endl;
		
	return 0;
}
