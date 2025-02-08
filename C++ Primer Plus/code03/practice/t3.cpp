#include<iostream>
int main()
{
	using namespace std;
	const int CONVERT=60;
	double degree_style;
	int degrees,minutes,seconds;
	cout<<"Enter a latitude in degrees, minutes, and seconds:\n";
	cout<<"First, enter the degrees: ";
	cin>>degrees;
	cout<<"Next, enter the minutes of arc: ";
	cin>>minutes;
	cout<<"Finally, enter the seconds of arc: ";
	cin>>seconds;
	degree_style=(double)seconds/(CONVERT*CONVERT)+(double)minutes/(CONVERT)+degrees;
	cout<<degrees<<" degrees, "
		<<minutes<<" minutes, "
		<<seconds<<" seconds = "
		<<degree_style<<" degrees.";
		
	return 0;
}
