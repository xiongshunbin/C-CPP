// *++pt	++*pt	(*pt)++		*pt++
#include<iostream>
int main()
{
	using namespace std;
	
	double arr[]={21.1,32.8,23.4,45.2,37.4};
	double*pt=arr;
	
	cout<<"*pt = "<<*pt<<endl;			// 21.1
	cout<<"*++pt = "<<*++pt<<endl;		// 32.8
	cout<<"++*pt = "<<++*pt<<endl;		// 33.8
	cout<<"(*pt)++ = "<<(*pt)++<<endl;	// 33.8
	cout<<"*pt = "<<*pt<<endl;			// 34.8
	cout<<"*pt++ = "<<*pt++<<endl;		// 34.8
	cout<<"*pt = "<<*pt<<endl;			// 23.4
	return 0;
}
