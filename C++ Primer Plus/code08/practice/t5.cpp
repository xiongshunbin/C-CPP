#include<iostream>
template <typename T>
T max5(T arr[]);
int main()
{
	using namespace std;
	int arr_i[5]={1,3,2,4,7};
	double arr_d[5]={22.3,37.9,44.2,13.8,38.5};
	cout<<"The max value of int array: "<<max5(arr_i)<<endl;
	cout<<"The max value of double array: "<<max5(arr_d)<<endl;
	
	return 0;
}
template <typename T>
T max5(T arr[])
{
	T max=arr[0];
	for(int i=1;i<5;i++)
	{
		if(arr[i]>max)
			max=arr[i];
	}
	return max;
}
