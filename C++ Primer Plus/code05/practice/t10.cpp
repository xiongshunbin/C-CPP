#include<iostream>
int main()
{
	using namespace std;
	int n;
	cout<<"Enter number of rows: ";
	cin>>n;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=n-i;j++)
			cout<<".";
		for(int j=1;j<=i;j++)
			cout<<"*";
		cout<<endl;
	}
	
	return 0;
}
