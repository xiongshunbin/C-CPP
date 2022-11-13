#ifndef __COMPLEX0_H__
#define __COMPLEX0_H__

#include <iostream>

using namespace std;

class complex
{
	private:
		double real;
		double imaginary;
	public:
		complex();
		complex(double r,double i);
		complex operator+(const complex & c) const;
		complex operator-(const complex & c) const;
		complex operator*(const complex & c) const;
		complex operator~() const;

		friend complex operator*(double x,const complex & c);
		friend istream & operator>>(istream & is,complex & c);
		friend ostream & operator<<(ostream & os,const complex & c);

};


#endif
