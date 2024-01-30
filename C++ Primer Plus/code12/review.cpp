#include <iostream>
#include <cstring>
using namespace std;

class nifty
{
	// data
	private:
		char *personality;
		int talents;
	// methods
	public:
		nifty();
		nifty(const char * s);
		nifty(const nifty & n);
		~nifty() { delete [] personality; }
		nifty & operator=(const nifty & n);
		friend ostream & operator<<(ostream & os, const nifty & n);
};

nifty::nifty()
{
	personality = NULL;
	talents = 0;
}

nifty::nifty(const char * s)
{
	personality = new char[strlen(s)+1];
	strcpy(personality, s);
	talents = 0;
}

nifty::nifty(const nifty & n)
{
	personality = new char[strlen(n.personality) + 1];
	strcpy(personality,n.personality);
	talents = 0;
}

nifty & nifty::operator=(const nifty & n)
{
	if(this == &n)
		return *this;
	delete [] personality;
	personality = new char[strlen(n.personality) + 1];
	strcpy(personality,n.personality);
	talents = 0;
	return *this;
}

ostream &operator<<(ostream & os, const nifty & n)
{
	os << n.personality << endl;
	os << n.talents << endl;
//	os << n;
	return os;
}

int main()
{
	nifty str1 = nifty("hello world");
	nifty str2 = str1;
	cout << str1;
	cout << str2;
}
