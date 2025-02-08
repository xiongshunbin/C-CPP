// const int *pt == int const *pt
// int *const pt
// const int *const pt
#include<iostream>
int main()
{
	using namespace std;
	
	int n=10;
	int m=100;
	const int* pt=&n;
	cout<<"1) n = "<<n<<endl;
//	*pt=20;
	pt=&m;
	n=100;
	cout<<"2) n = "<<n<<endl;
	cout<<"*pt = "<<*pt<<endl;
	cout<<"m = "<<m<<endl;
	cout<<"------------------------\n";
	
	int i=10;
	int j=100;
	
	int* const p=&i;
	cout<<"1) i= "<<i<<endl;
	*p=20;
	cout<<"2) i= "<<i<<endl;
//	p=&j;
	
	return 0;
}
