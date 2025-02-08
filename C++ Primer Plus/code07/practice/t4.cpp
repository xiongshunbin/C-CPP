// lotto.cpp -- probability of winning
#include<iostream>
// Note: some implementations requir double instead of long double
long double probability(unsigned numbers,unsigned picks);
int main()
{
	using namespace std;
	long double field=probability(47,5);
	long double special=probability(27,1);
	long double result=field*special;
	cout<<"probability is: "<<result<<endl;
	cout<<"Bye\n";
	
	return 0;
}

// the following function calculates the probability of picking picks
// numbers correctly from numbers choices
long double probability(unsigned numbers,unsigned picks)
{
	long double result=1.0;	// here come some local variables
	long double n;
	unsigned p;
	for(n=numbers,p=picks;p>0;n--,p--)
		result=result*n/p;
	return result;
}
