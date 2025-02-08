// if.cpp -- using the if statement
#include<iostream>

int main()
{
	using std::cin;		// using declarations
	using std::cout;
	
	int spaces=0;
	int total=0;
	char ch;
	
	cin.get(ch);
	while(ch!='.')		// quit at end of sentence
	{
		if(ch==' ')		// check if ch is a space
			++spaces;
		++total;		//done every time
		cin.get(ch);
	}
	cout<<spaces<<" spaces, "<<total;
	cout<<" characters total in sentence\n";
	
	return 0;
}
