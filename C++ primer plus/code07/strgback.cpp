// strgback -- a function that returns a pointer to char
#include<iostream>
char * buildstr(char ch,int n);	// prototype
int main()
{
	using namespace std;
	char ch;
	int times;
	cout<<"Enter a character: ";
	cin>>ch;
	cout<<"Enter an integer: ";
	cin>>times;
	char *ps=buildstr(ch,times);
	cout<<ps<<endl;
	delete [] ps;		// free memory
	ps=buildstr('+',20);	// reuse pointer
	cout<<ps<<"-DONE-"<<ps<<endl;
	delete [] ps;		// free memory

	return 0;
}
char * buildstr(char ch,int n)
{
	char* pstr=new char[n+1];
	pstr[n]='\0';	// terminate string
	while(n-->0)
		pstr[n]=ch;	// fill rest of string
	return pstr;
}
