#include<iostream>

const int Max=40;
int Fill_array(double ar[],int limit);
void Show_array(double ar[],int n);
void Reverse_array(double ar[],int n);

int main()
{
	using namespace std;
	double arr[Max];
	int size=Fill_array(arr,Max);
	
	Show_array(arr,size);
	Reverse_array(arr,size);
	Show_array(arr,size);
	
	Reverse_array(arr+1,size-2);
	Show_array(arr,size);
	
	return 0;
}

int Fill_array(double ar[],int limit)
{
	using namespace std;
	int i;
	double temp;
	for(i=0;i<limit;i++)
	{
		cout<<"Enter a number:";
		cin>>temp;
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			break;
		}
		else
			ar[i]=temp;
	}
	return i;
}

void Show_array(double ar[],int n)
{
	using namespace std;
	cout<<"The array content: ";
	for(int i=0;i<n;i++)
		cout<<ar[i]<<" ";
	cout<<endl;
}

void Reverse_array(double ar[],int n)
{
	double temp;
	for(int i=0;i<n/2;i++)
	{
		temp=ar[i];
		ar[i]=ar[n-1-i];
		ar[n-i-1]=temp;
	}
}

