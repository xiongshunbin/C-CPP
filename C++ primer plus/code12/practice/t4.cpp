// stacker.cpp -- testing the Satck class
#include <iostream>
#include "stack.h"

using namespace std;

int main()
{
	int max = 5;
	Item item;

	Stack st1(max);
	cout << "Enter " << max << " numbers you want to push to stack: " << endl;
	for(int i = 0; i < max; i++)
	{
		cin >> item;
		st1.push(item);
	}
	Stack st2 = st1;
	Stack st3;
	st3 = st1;
	while(!st2.isempty())
	{
		st2.pop(item);
		cout << item << " is poped" << endl;
	}
	while(!st3.isempty())
	{
		st3.pop(item);
		cout << item << " is poped" << endl;
	}
	return 0;
}
