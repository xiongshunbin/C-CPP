#include <iostream>
#include "list.h"

using namespace std;

int main()
{
	List list;
	Item num;
	cout<<"Please enter some numbers: "<<endl;
	for(int i=0;i<5;i++)
	{
		cout<<"#"<<i+1<<": ";
		cin>>num;
		list.add(num);
	}
	cout<<"Show the items: ";
	list.visit(visit_item);

	return 0;
}
