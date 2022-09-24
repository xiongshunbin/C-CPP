#include<iostream>
long long factorial(int n);
int main()
{
	using namespace std;
	int number;
	long long result;
	cout<<"Enter a number for factorial: ";
	while(cin>>number)
	{
		result=factorial(number);
		cout<<number<<"! = "<<result<<endl;
		cout<<"Enter next number: ";
	}
	
	return 0;
}

long long factorial(int n)
{
	if(n==0)
		return 1;
	else
		return n*factorial(n-1);
}
