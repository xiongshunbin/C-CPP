// express.cpp -- values of expressions
#include<iostream>
int main()
{
	using namespace std;
	int x;
	
	cout<<"The express x = 100 has the value ";
	cout<<(x=100)<<endl;
	cout<<"Now x = "<<x<<endl;
	cout<<"The express x < 3 has the value ";
	cout<<(x<3)<<endl;
	cout<<"The express x > 3 has the value ";
	cout<<(x>3)<<endl;
	cout.setf(ios_base::boolalpha);	// a newer C++ feature
	cout<<"The express x < 3 has the value ";
	cout<<(x<3)<<endl;
	cout<<"The express x > 3 has the value ";
	cout<<(x>3)<<endl;
		
	return 0;
}
