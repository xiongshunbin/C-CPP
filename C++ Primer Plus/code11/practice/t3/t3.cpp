// t3.cpp -- using the Vector class
// compile with the vect.cpp file
#include <iostream>
#include <cstdlib>	// rand(), srand() prototypes
#include <ctime>	// time() prototype
#include "vect.h"

int main()
{
	using namespace std;
	using VECTOR::Vector;
	srand(time(0));	// seed random-number generator
	double direction;
	Vector step;
	Vector result(0.0,0.0);
	unsigned long steps=0;
	unsigned int N;
	double target;
	double dstep;
	unsigned int max=0;
	unsigned int min=0;
	unsigned int sum=0;

	cout<<"How many times do you wan to try? ";
	cin>>N;
	cout<<"Enter target distance: ";
	cin>>target;
	cout<<"Enter the step length: ";
	cin>>dstep;
	for(int i=0;i<N;i++)
	{
		while(result.magval()<target)
		{
			direction=rand()%360;
			step.reset(dstep,direction,Vector::POL);
			result=result+step;
			steps++;
		}
		max=(max>steps)?max:steps;
		if(min==0)
			min=max;
		else
			min=(min<steps)?min:steps;
		sum+=steps;
		steps=0;
		result.reset(0.0,0.0);
	}
	cout<<"The number of max steps is: "<<max<<endl;
	cout<<"THe number of min steps is: "<<min<<endl;
	cout<<"The average of steps is: "<<(double)sum/N<<endl;
	cout<<"Bye!\n";

	return 0;
}
