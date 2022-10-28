#include <iostream>

using namespace std;

class Num
{
	public:
		int n;
	public:
		Num();
		Num(int m);
		void Show() const;
};

Num::Num()
{
	n=0;
}

Num::Num(int m)
{
	n=m;
}

void Num::Show() const
{
	cout<<"n = "<<n<<endl;
}

Num operator+(const Num & m,const Num & l)
{
	Num temp;
	temp.n=m.n+l.n;
	return temp;
}

int main()
{
	Num a(10);
	a.Show();
	Num b(20);
	b.Show();

	Num c;
	c=a+b;
	c.Show();
}

