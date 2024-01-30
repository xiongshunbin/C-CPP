// strngbad.h -- flawed string class definition
#include <iostream>
#ifndef __STRNGBAD_H__
#define __STRNGBAD_H__
class StringBad
{
	private:
		char *str;					// pointer to string
		int len;					// length of string
		static int num_strings;		// number of objects
	public:
		StringBad(const char * s);	// constructor
		StringBad();				// default constructor
		StringBad(const StringBad & st);
		~StringBad();				// destructor

		StringBad & operator=(const StringBad &st);
		// friend function
		friend std::ostream & operator<<(std::ostream & os,const StringBad & st);
};

#endif
