// arrfun3.cpp -- array functions and const
#include<iostream>
const int Max=5;
// function prototypes
int fill_array(double arr[],int limit);
void show_array(const double arr[],int n);	// don't change data
void revalue(double r,double arr[],int n);

int main()
{
	using namespace std;
	double properties[Max];
	
	int size=fill_array(properties,Max);
	show_array(properties,size);
	if(size>0)
	{
		cout<<"Enter revaluation factor: ";
		double factor;
		while(!(cin>>factor))	// bad input
		{
			cin.clear();
			while((cin.get())!='\n');
			cout<<"Bad inpue, please enter again: ";
		}
		revalue(factor,properties,size);
		show_array(properties,size);
	}
	cout<<"Done.\n";
	cin.get();
	cin.get();
	
	return 0;
}

int fill_array(double arr[],int limit)
{
	using namespace std;
	double temp=0;
	int i;
	for(i=0;i<limit;i++)
	{
		cout<<"Enter value #"<<i+1<<": ";
		cin>>temp;
		if(!cin)
		{
			cin.clear();
			while((cin.get())!='\n');
			cout<<"Bad input: input process terminated.";
			break;
		}else if(temp<0)
			break;
		else
			arr[i]=temp;
	}	
	return i;
}
// the following function can use, but not alter
// the array whose address is ar
void show_array(const double arr[],int n)
{
	using namespace std;
	for(int i=0;i<n;i++)
	{
		cout<<"Property #"<<i+1<<": $";
		cout<<arr[i]<<endl;
	}
}

// multiplies each element of ar[] by r
void revalue(double r,double arr[],int n)
{
	for(int i=0;i<n;i++)
		arr[i]*=r;
}
