#include<iostream>

using namespace std;

const int Max=10;
int fill(double ar[],int n);
void show(const double *ar,int n);
double average(double ar[],int n);

int main()
{
	double scores[Max];
	int size=fill(scores,Max);
	if(size>0)
	{
		show(scores,size);
		cout<<"Average: "<<average(scores,size)<<endl;
	}
	else
		cout<<"There is no scores";
		
	return 0;
}

int fill(double ar[],int n)
{
	int i=0;
	double temp;
	for(i=0;i<n;i++)
	{
		cout<<"Enter score NO."<<i+1<<": ";
		cin>>temp;
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			cout<<"Invalid input, terminate."<<endl;
			break;
		}
		else if(temp<0)
			break;
		ar[i]=temp;
	}
	return i;
}

void show(const double *ar,int n)
{
	cout<<"golf result: ";
	for(int i=0;i<n;i++)
		cout<<ar[i]<<' ';
	cout<<endl;
}

double average(double ar[],int n)
{
	double average,sum=0;
	for(int i=0;i<n;i++)
		sum+=ar[i];
	average=sum/n;
	return average;
}
