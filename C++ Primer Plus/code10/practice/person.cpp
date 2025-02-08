#include <iostream>
#include "person.h"

using std::cout;
using std::endl;

Person::Person(const std::string & ln,const char * fn)
{
	lname=ln;
	strcpy(fname,fn);
}

void Person::Show() const
{
	if(lname==""&&fname[0]=='\0')
		cout<<"No name!"<<endl;
	else
		cout<<"Name: "<<fname<<" "<<lname<<endl;
}
void Person::FormalShow() const
{
	if(lname==""&&fname[0]=='\0')
		cout<<"No name!"<<endl;
	else
		cout<<"Name: "<<lname<<", "<<fname<<endl;
}

