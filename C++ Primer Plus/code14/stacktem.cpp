// stacktem.cpp -- testing the template stack class
#include <iostream>
#include <string>
#include <cctype>// or ctype.h
#include "stacktp.h"
using std::cin;
using std::cout;

int main()
{
	Stack<std::string> st;// create an empty stack
	char ch;
	std::string po;
	cout<<"Please enter A to add a purchase order,\n"
	    <<"P to process a PO, or Q to quit.\n";
	while(cin>>ch && toupper(ch)!='Q')
	{
		while(cin.get()!='\n')
			continue;
		if(!isalpha(ch))
		{
			cout<<'\a';
			continue;
		}
		switch(ch)
		{
			case 'A':
			case 'a':cout<<"Enter a PO number to add: ";
				cin>>po;
				if(st.isfull())
					cout<<"stack already full\n";
				else
					st.push(po);
				break;
			case 'p':
			case 'P':if(st.isempty())
					cout<<"stack already empty\n";
				 else{
					st.pop(po);
					cout<<"PO #"<<po<<" popped\n";
				}
				break;
		}
		cout<<"Please enter A to add a purchase order,\n"
		      "P to process a PO, or Q to quit.\n";
	}
	cout<<"Bye\n";
	return 0;
}
