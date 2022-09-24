#include<iostream>
#include<cstring>
template <typename T>
T maxn(T arr[],int n);
template <> char * maxn<char *>(char *str[], int n);
int main()
{
	using namespace std;
	int arr_i[6]={1,3,2,4,7,5};
	double arr_d[4]={22.3,37.9,44.2,13.8};
	const char *str[5]={"Hello world!",
				  "Good morning!",
				  "yyds!",
				  "I love you!",
				  "What's this?"};
	cout<<"The max value of int array: "<<maxn(arr_i,6)<<endl;
	cout<<"The max value of double array: "<<maxn(arr_d,4)<<endl;
	cout<<"The max value of str: "<<maxn(str,5)<<endl;
	
	return 0;
}
template <typename T>
T maxn(T arr[],int n)
{
	T max=arr[0];
	for(int i=1;i<n;i++)
	{
		if(arr[i]>max)
			max=arr[i];
	}
	return max;
}
template <> char * maxn<char *>(char *str[], int n)
{
	int pos=0;
	for(int i=1;i<n;i++)
	{
		if(strlen(str[pos])<strlen(str[i]))
			pos=i;
	}
	return str[pos];
}
