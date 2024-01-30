// arfupt.cpp -- an array of function pointers
#include<iostream>
// various notations, same signatures
const double *f1(const double*ar,int n);
const double *f2(const double ar[],int n);
const double *f3(const double ar[],int n);

int main()
{
	using namespace std;
	double av[3]={1112.3,1542.6,2227.9};
	
	// part1:
	//p1(p2):pointer to a function
	const double*(*p1)(const double*,int)=f1;
	auto p2=f2;		//C++11 automatic type deduction
	// pre-C++11 can use the following code instead
	// const double *(*p2)(const double *,int)=f2; 
	cout<<"PART1：------------------"<<endl;
	cout<<"Using pointers to functions:\n";
	cout<<"  Address     Value"<<endl;
	cout<<(*p1)(av,3)<<": "<<*((*p1)(av,3))<<endl;
	cout<<p2(av,3)<<": "<<*(p2(av,3))<<endl;
	
	// part2
	// pa(pb) an array of pointers
	cout<<"\nPART2：------------------"<<endl;

	// auto doesn't work with list initialization
	const double *(*pa[3])(const double *,int)={f1,f2,f3};
	// but it does work for initalizing to a single value
	// pb a pointer to first element of pa
	auto pb=pa;
	// pre-C++11 can use the following code instead
	// const double *(**pb)(const double *,int)=pa;
	cout<<"Using an array of pointer to functions:\n";
	cout<<"  Address     Value"<<endl;
	for(int i=0;i<3;i++)
		cout<<pa[i](av,3)<<": "<<*(pa[i](av,3))<<endl;
	cout<<"\nUsing a pointer to a pointer to a functions:\n";
	cout<<"  Address     Value"<<endl;
	for(int i=0;i<3;i++)
		cout<<pb[i](av,3)<<": "<<*(pb[i](av,3))<<endl;
	
	// part3
	// pc(pd) is a pointer to an array of function pointers
	cout<<"\nPART3：------------------"<<endl;
	cout<<"Using pointers to an array of pointers:\n";
	cout<<"  Address     Value"<<endl;
	// easy way to declare pd
	auto pc=&pa;
	//  pre-C++11 can use the following code instead
	// const double *(*(*pd)[3])(const double*,int)=&pa;
	cout<<(*pc)[0](av,3)<<": "<<*(*pc)[0](av,3)<<endl;
	// hard way to declare pd
	const double *(*(*pd)[3])(const double*,int n )=&pa;
	// store return value in pdb
	const double* pdb=(*pd)[1](av,3);
	cout<<pdb<<": "<<*pdb<<endl;
	// cout<<(*pd)[2](av,3)<<": "<<*(*pd)[2](av,3)<<endl;
	// alternative notation
	cout<<(*(*pd)[2])(av,3)<<": "<<*(*(*pd)[2])(av,3)<<endl;
	// cin.get();
	
	return 0;
}
// some rather dull functions

const double *f1(const double*ar,int n)
{
	return ar;
}

const double *f2(const double ar[],int n)
{
	return ar+1;
}

const double *f3(const double ar[],int n)
{
	return ar+2;
}
