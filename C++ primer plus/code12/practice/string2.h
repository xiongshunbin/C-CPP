// string2.h -- fixed and augmented string class definition

#ifndef __STRING_2_H__
#define __STRING_2_H__
#include <iostream>
using std::ostream;
using std::istream;

class String
{
	private:
		char *str;			// pointer to string
		int len;			// length of string
		static int num_strings;		// number of objects
		static const int CINLIM=80;	// cin input limit
	public:
	// constructors and other mathods
		String(const char * s);		// constructor
		String();			// default constructor
		String(const String & st);	// copy constructor
		~String();			// destructor
		int length () const { return len; }
		void stringlow();
		void stringup();
		int has(char c) const;
	// overloaded operator methods
		String & operator=(const String &);
		String & operator=(const char *);
		char & operator[](int i);
		const char & operator[](int i) const;
	// overloaded operator friends
		friend bool operator<(const String &st1,const String &st2);
		friend bool operator>(const String &st1,const String &st2);
		friend bool operator==(const String &st1,const String &st2);
		friend ostream & operator<<(ostream & os,const String & st);
		friend istream & operator>>(istream & is,String & st);
		friend const String operator+(const String &st1,const String &st2);
	// static function
		static int HowMany();
};

#endif
