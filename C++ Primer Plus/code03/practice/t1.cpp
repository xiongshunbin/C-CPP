#include<iostream>
int main()
{
	using namespace std;
	const int inch_per_foot=12;
	
	int height;
	cout<<"Enter your height in inch:_______\b\b\b\b\b";
	cin>>height;
	int foot=height/inch_per_foot;
	int inch=height%inch_per_foot;
	cout<<height<<" inch are "<<foot<<" foot, "<<inch<<" inch.\n";
	
	return 0;
}
