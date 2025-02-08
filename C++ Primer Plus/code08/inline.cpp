// inline.cpp -- using an inline function
#include<iostream>

// an inline function definition
inline double square(double x){return x*x;};
#define squ(x) ((x)*(x))

int main()
{
	using namespace std;
	double a,b;
	double c=13.0;
	
	a=square(5.0);
	b=square(4.5+7.5);
	cout<<"a = "<<a<<", b = "<<b<<endl;
	cout<<"c = "<<c;
	cout<<", c square = "<<square(c++)<<endl;
	cout<<"Now c = "<<c<<endl;
	
	double n=13.0;
	cout<<"n squ = "<<squ(n++);
	cout<<", n = "<<n<<endl;
	
	return 0;
}
