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
	Pizza* pizza=new Pizza;
	cout<<"Enter the size of pizza in inches:"<<endl;
	(cin>>pizza->diameter).get();
	cout<<"Enter the pizza's company:\n";
	cin.getline(pizza->company,20);
	cout<<"Enter the weight of the pizza in pounds:\n";
	cin>>pizza->weight;
	cout<<"Pizza'company:"<<pizza->company<<", "
		<<"size: "<<pizza->diameter<<", "
		<<"weight: "<<pizza->weight<<"."<<endl;
	delete pizza;
	
	return 0;
}
