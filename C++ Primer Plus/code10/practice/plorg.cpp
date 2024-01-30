#include "plorg.h"

Plorg::Plorg(const char *str,int ci)
{
	strcpy(name,str);
	CI=ci;
}

void Plorg::reset(int ci)
{
	CI=ci;
}

void Plorg::show() const
{
	cout<<"Name: "<<name<<", CI = "<<CI<<endl;
}
