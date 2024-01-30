// arrobj.cpp -- functions with array objects (C++11)
#include<iostream>
#include<array>
#include<string>
// constant data
const int Seasons=4;
const char* Snames[]=
{"Spring","Summer","Fall","Winter"};

struct Spend
{
	double money[Seasons];
};

// function to modify array object
void fill(double arr[],int size);
void show(const double arr[],int size);

int main()
{
	Spend expense;
	fill(expense.money,Seasons);
	show(expense.money,Seasons);
	
	return 0;
}
void fill(double arr[],int size)
{
	using namespace std;
	for(int i=0;i<size;i++)
	{
		cout<<"Enter "<<Snames[i]<<" expenses: ";
		cin>>arr[i];
	}
}

void show(const double arr[],int size)
{
	using namespace std;
	double total=0.0;
	cout<<"\nEXPENSES\n";
	for(int i=0;i<size;i++)
	{
		cout<<Snames[i]<<": $"<<arr[i]<<endl;
		total+=arr[i];
	}
	cout<<"Total Expenses: $"<<total<<endl;
}
