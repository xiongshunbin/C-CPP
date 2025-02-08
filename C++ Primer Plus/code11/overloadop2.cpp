// overloadop2.cpp
#include <iostream>

using namespace std;

class Num
{
	public:
		int n;
	public:
		Num();
		Num(int m);
//		void operator=(int l);
};

Num::Num()
{
	n=0;
	cout<<"hello world!"<<endl;
}
Num::Num(int m)
{
	n=m;
	cout<<"Good morning!"<<endl;
}

void operator=(Num & num,int l)
{
	num.n=l;
	cout<<"How are you?"<<endl;
}

int main()
{
	Num a;

	a=10;

	return 0;
}
