// calling.cpp -- defining, prototyping, and calling a function
#include<iostream>
using namespace std;
void simple();	// function prototype
int main()
{
	cout<<"main() will call the simple() function:\n";
	simple();	// function call
	cout<<"main() is finished with the simple() function.\n";
	//cin.get();
	
	return 0;
}

// function definition
void simple()
{
	cout<<"I'm but a simple() function.\n";
}
