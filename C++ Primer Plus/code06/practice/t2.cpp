#include<iostream>
#include<array>
const int Max=10;

int main()
{
	using namespace std;
	array<double,Max> donation;
	int count=0;
	double sum=0.0;
	double average=0.0;
	int bigger=0;
	
	cout<<"Please enter the double numberial."<<endl;
	cout<<"number #1:";
	while(count<Max&&(cin>>donation[count]))
	{
		sum+=donation[count];
		if(++count<Max)
			cout<<"number #"<<count+1<<":";
	}
	average=sum/count;
	for(int i=0;i<count;i++)
	{
		if(donation[i]>average)
		bigger++;
	}
	cout<<"Average is: "<<average<<endl;
	cout<<bigger<<" number(s) are bigger than average."<<endl;
	
	return 0;
}
