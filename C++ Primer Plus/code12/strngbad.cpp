// strngbad.cpp -- StringBad class methods
#include "strngbad.h"
#include <cstring>			// string.h for some
using std::cout;

// initialzing static class member
int StringBad::num_strings=0;

// class methods
// construct StringBad from C string
StringBad::StringBad(const char * s)
{
	len=strlen(s);					// set size
	str=new char[len+1];			// allot storage
	std::strcpy(str,s);				// initialize pointer
	num_strings++;					// set object count
	cout<<num_strings<<": \""<<str
	    <<"\" object created\n";	// For Your Information
}

StringBad::StringBad()				// default constructor
{
	len=4;
	str=new char[4];
	std::strcpy(str,"C++");			// default string
	num_strings++;
	cout<<num_strings<<": \""<<str
	    <<"\" default object created\n";	// FYI
}

StringBad::StringBad(const StringBad & st)
{
	len=st.len;
	str=new char[len+1];
	strcpy(str,st.str);
	num_strings++;
	cout<<num_strings<<": \""<<str
	    <<"\" object created\n";
}

StringBad::~StringBad()						// necessary destructor
{
	cout<<"\""<<str<<"\" object deleted, ";	//FYI
	--num_strings;							// required
	cout<<num_strings<<" left\n";			// FYI
	delete [] str;							// required
}

StringBad & StringBad::operator=(const StringBad & st)
{
	if(this==&st)
		return *this;
	delete [] str;
	len=st.len;
	str=new char[len+1];
	strcpy(str,st.str);
	return *this;
}

std::ostream & operator<<(std::ostream & os,const StringBad & st)
{
	os<<st.str;
	return os;
}
