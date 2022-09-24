// arrfun3.cpp -- array functions and const
#include<iostream>
const int Max=5;
// function prototypes
double *fill_array(double *begin,double *end);
void show_array(double *begin,double *end);	// don't change data
void revalue(double r,double *begin,double *end);

int main()
{
	using namespace std;
	double properties[Max];
	
	double *pa=fill_array(properties,properties+Max);
	show_array(properties,pa);
	if(pa!=properties)
	{
		cout<<"Enter revaluation factor: ";
		double factor;
		while(!(cin>>factor))	// bad input
		{
			cin.clear();
			while((cin.get())!='\n');
			cout<<"Bad inpue, please enter again: ";
		}
		revalue(factor,properties,pa);
		show_array(properties,pa);
	}
	cout<<"Done.\n";
	cin.get();
	cin.get();
	
	return 0;
}

double *fill_array(double *begin,double *end)
{
	using namespace std;
	double temp;
	double *pt;
	for(pt=begin;pt!=end;pt++)
	{
		cout<<"Enter value #"<<pt-begin+1<<": ";
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
			*pt=temp;
	}	
	return pt;
}
// the following function can use, but not alter
// the array whose address is ar
void show_array(double *begin,double *end)
{
	using namespace std;
	for(double *i=begin;i!=end;i++)
	{
		cout<<"Property #"<<i-begin+1<<": $";
		cout<<*i<<endl;
	}
}

// multiplies each element of ar[] by r
void revalue(double r,double *begin,double *end)
{
	for(double *i=begin;i!=end;i++)
		*i*=r;
}
