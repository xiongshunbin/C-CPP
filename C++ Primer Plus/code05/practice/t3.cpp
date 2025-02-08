#include<iostream>
int main()
{
	using namespace std;
	int num,sum=0;
	do{
		cout<<"Enter a number to add: ";
		cin>>num;
		sum+=num;
		cout<<"sum = "<<sum<<endl;
		
	}while(num!=0);
	
	return 0;
}
