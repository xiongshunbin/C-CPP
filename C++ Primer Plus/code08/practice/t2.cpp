#include<iostream>
#include<cstring>
const int Size=50;
struct CandyBar
{
	char brand[Size];
	double weight;
	int heat;
};

void set_value(CandyBar &candybar,const char * str="Millennium Munch",const double w=2.85,const int h=350);
void show(CandyBar & candybar);

int main()
{
	using namespace std;
	
	CandyBar bar;
	set_value(bar);
	show(bar);
	
	return 0;
}
void set_value(CandyBar & candybar,const char * str,const double w,const int h)
{
	strcpy(candybar.brand,str);
	candybar.weight=w;
	candybar.heat=h;
}
void show(CandyBar & candybar)
{
	using namespace std;
	cout<<"Brand: "<<candybar.brand<<endl;
	cout<<"Weight: "<<candybar.weight<<endl;
	cout<<"Heat: "<<candybar.heat<<endl;
}
