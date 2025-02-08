#ifndef __PLORG_H__
#define __PLORG_H__

#include<iostream>
#include <cstring>
using namespace std;

const int SIZE=19;

class Plorg
{
	private:
		char name[SIZE];
		int CI;
	public:
		Plorg(const char *str="Plorga",int ci=50);
		void reset(int ci);
		void show() const;
};

#endif
