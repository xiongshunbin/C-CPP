#include<iostream>
#include<string>
using namespace std;

struct Patrons
{
	string name;
	double donation;
	
};
int main()
{
	int number;
	bool empty=true;
	cout<<"Enter the number of patrons: ";
	cin>>number;
	Patrons* p=new Patrons[number];
	
	for(int i=0;i<number;i++)
	{
		cout<<"donor #"<<i+1<<":"<<endl;
		cout<<"\tname: ";
		cin>>p[i].name;
		cout<<"\tdonation: ";
		cin>>p[i].donation;
	}
	cout<<"----------Grand Patrons----------"<<endl;
	for(int i=0;i<number;i++)
	{
		if(p[i].donation>10000)
		{
			cout<<p[i].name<<": "<<p[i].donation<<endl;
			empty=false;
		}
	}
	if(empty==true)
		cout<<"none"<<endl;
	
	cout<<"-------------Patrons-------------"<<endl;
	empty=true;
	for(int i=0;i<number;i++)
	{
		if(p[i].donation<=10000)
		{
			cout<<p[i].name<<": "<<p[i].donation<<endl;
			empty=false;
		}
	}
	if(empty==true)
		cout<<"none";
	
	delete[] p;
	
	return 0;
}
