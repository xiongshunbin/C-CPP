#include<iostream>
using namespace std;

double add(double x,double y);
double subtract(double x,double y);
double multiply(double x,double y);
double calculate(double x,double y,double (*pt)(double,double));

int main()
{
	const char symbol[3]={'+','-','x'};
	double x,y;
	double result=calculate(2.5,10.4,add);
	cout<<"The result of add is: "<<result<<endl;
	result=calculate(10.4,2.5,subtract);
	cout<<"The result of subtract is: "<<result<<endl;
	double (*pf[3])(double,double)={add,subtract,multiply};
	cout<<"Enter two number: ";
	while(cin>>x>>y){
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			break;
		}
		for(int i=0;i<3;i++)
		{
			result=calculate(x,y,pf[i]);
			cout<<x<<symbol[i]<<y<<" = "<<result<<endl;
		}
		cout<<"Enter two number: ";
	}
	
	return 0;
}

double add(double x,double y)
{
	return x+y;
}

double subtract(double x,double y)
{
	return x-y;
}

double multiply(double x,double y)
{
	return x*y;
}

double calculate(double x,double y,double (*pt)(double,double))
{
	return pt(x,y);
}


