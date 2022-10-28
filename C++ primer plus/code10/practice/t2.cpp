#include <iostream>
#include "person.h"
using namespace std;
int main()
{
	Person one;			// use default constructor
	Person two("Smythecraft");	// use #2 with one default format
	Person three("Dimwiddy","Sam");	// use #2, no defaults
	one.Show();
	one.FormalShow();

	cout<<endl;
	two.Show();
	two.FormalShow();

	cout<<endl;
	three.Show();
	three.FormalShow();

	return 0;
}

