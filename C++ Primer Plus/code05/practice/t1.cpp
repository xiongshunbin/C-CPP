#include<iostream>
int main()
{
	using namespace std;
	int min,max,sum=0;
	
	cout<<"Enter the min number: ";
	cin>>min;
	cout<<"Enter the max number: ";
	cin>>max;
	for(int i=min;i<=max;i++){
		sum+=i;
	}
	cout<<"The sum = "<<sum<<endl;
	
	return 0;
}
