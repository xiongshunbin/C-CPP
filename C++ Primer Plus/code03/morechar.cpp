// morechar.cpp -- the type and int type contrasted

#include<iostream>

int main()
{
	using namespace std;
	char ch='M';	//assign ASCII code for M to ch
	int i=ch;		//store same code in an int
	cout<<"The ASCII code for "<<ch<<" is "<<i<<endl;
	
	cout<<"Add one to the character code:"<<endl;
	ch=ch+1;	//change character code in ch
	i=ch;		//save new character code in i
	cout<<"The ASCII code for "<<" is "<<i<<endl;
	
	//using the cout.put() member function to display a char
	cout<<"Displaying char ch using cout.put(ch): ";
	cout.put(ch);
	
	//using cout.put() to display a char consatnt
	cout.put('!');
	
	cout<<endl<<"Done"<<endl;
	return 0;
}
