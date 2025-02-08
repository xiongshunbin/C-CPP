// arith.cpp -- some C++ arithmetic

#include<iostream>
int main()
{
	using namespace std;
	
	float hats,heads;
//	cout.setf(ios_base::fixed,ios_base::floatfield);	//fixed-point
	cout<<"Enter a number:";
	cin>>hats;
	cout<<"Enter another number:";
	cin>>heads;
	
	cout<<"hats = "<<hats<<"; heads = "<<heads<<endl;
	cout<<"hats+heads="<<hats+heads<<endl;
	cout<<"hats-heads="<<hats-heads<<endl;
	cout<<"hats*heads="<<hats*heads<<endl;
	cout<<"hats/heads="<<hats/heads<<endl;
	
	double a=1.11111111;
	double b=1.11111111;
	cout<<"a+b="<<a+b<<endl;
	cout<<12.34556<<endl;
	return 0;
}
