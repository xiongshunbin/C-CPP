#include "cow.h"
#include <cstring>

Cow::Cow()
{
	name[0] = '\0';
	hobby = NULL;
	weight = 0.0;
}

Cow::Cow(const char * nm, const char * ho, double wt)
{
	strncpy(name, nm, 20);
	if(strlen(nm) >= 20)
		name[19] = '\0';
	hobby = new char[strlen(ho) + 1];
	strcpy(hobby, ho);
	weight = wt;
}

Cow::Cow(const Cow & c)
{
	strcpy(name, c.name);
	hobby = new char[strlen(c.hobby) + 1];
	strcpy(hobby, c.hobby);
	weight = c.weight;
}

Cow::~Cow()
{
	delete [] hobby;
}

Cow & Cow::operator=(const Cow & c)
{
	if(this == &c)
		return *this;
	delete [] hobby;
	strcpy(name, c.name);
	hobby = new char[strlen(c.hobby) + 1];
	strcpy(hobby, c.hobby);
	weight = c.weight;
	return *this;
}

void Cow::ShowCow() const
{
	cout << "The cow's name: " << name << endl;
	cout << " hobby: " << hobby << endl;
	cout << " weight = "<< weight << endl;
}
