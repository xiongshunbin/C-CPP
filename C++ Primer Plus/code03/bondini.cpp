// bondini.cpp -- using escape sequences
#include<iostream>
int main()
{
	using namespace std;
	cout<<"\aOPeration \"HyperHype\" is now activated!\n";
	cout<<"Enter your agent code:________\b\b\b\b\b\b\b\b";
	long code;
	cin>>code;
	cout<<"\aYou entered "<<code<<"...\n";
	cout<<"\aCode verifiled! Proceed with Plan z3!\n";
	return 0;
}
