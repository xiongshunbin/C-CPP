// distinguish between sizeof and strlen
#include<iostream>
#include<cstring>
#include<string>
int main()
{
	using namespace std;

	int a[] = { 1,2,3,4 };
	int *b=a;
	cout<<"sizoef b = "<<sizeof(b)<<endl;
	cout<<"sizeof(a) = "<<sizeof(a)<<endl;			// 16
	cout<<"sizeof(*a) = "<<sizeof(*a)<<endl;		// 4
	cout<<"sizeof(&a) = "<<sizeof(&a)<<endl;		// 8
	cout<<"sizeof(*&a) = "<<sizeof(*&a)<<endl;		// 16
	cout<<"sizeof(&a+1) = "<<sizeof(&a+1)<<endl;	// 8
	
	char str1[]="string";
	char str2[]={'s','t','r','i','n','g','\0'};
	string str3="string";
//	char * pc=str3;
	
	return 0;
}

