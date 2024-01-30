// swaps.cpp -- swapping with references and with pointers
#include<iostream>

void swapr(int & a,int & b);	// a, b are aliases
void swapp(int *p,int *q);
void swapv(int a,int b);

int main()
{
	using namespace std;
	int wallet1=300;	
	int wallet2=350;
	cout<<"wallet1 = $"<<wallet1;
	cout<<" wallet2 = $"<<wallet2<<endl;
	
	cout<<"Using references to swap contents:\n";
	swapr(wallet1,wallet2);	// pass variables
	cout<<"wallet1 = $"<<wallet1;
	cout<<" wallet2 = $"<<wallet2<<endl;
	
	cout<<"Using pointers to swap contents again:\n";
	swapp(&wallet1,&wallet2);	// pass addresses of variables
	cout<<"wallet1 = $"<<wallet1;
	cout<<" wallet2 = $"<<wallet2<<endl;
	
	cout<<"Trying to use passing by value:\n";
	swapv(wallet1,wallet2);		// pass values of variables
	cout<<"wallet1 = $"<<wallet1;
	cout<<" wallet2 = $"<<wallet2<<endl;
	
	return 0;
}
void swapr(int & a,int & b)	// use references
{
	int temp;
	
	temp=a;	// use a, b for values of variables
	a=b;
	b=temp;
}

void swapp(int *p,int *q)	// use pointer
{
	int temp;
	
	temp=*p;		// use *p, *q for values of variables
	*p=*q;
	*q=temp;
}

void swapv(int a,int b)	// try use values 
{
	int temp;
	
	temp=a;		// use a, b for values of variables
	a=b;
	b=temp;
}
