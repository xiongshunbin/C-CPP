#include<iostream>
void show(const char * str,int n=0);
int main()
{
	using namespace std;
	show("Hello!");
	show("How are you?");
	show("I'm fine, thanks.",3);
	return 0;
}
void show(const char * str,int n)
{
	using namespace std;
	static int num=0;
	num++;
	if(n==0)
		cout<<str<<endl;
	else
	{
		for(int i=0;i<num;i++)
			cout<<str<<endl;
	}
}
