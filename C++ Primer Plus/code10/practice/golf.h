#ifndef __GOLF_H__
#define __GOLF_H__

#include <iostream>
#include <cstring>
using namespace std;

const int Len=40;

class Golf
{
	private:
		char fullname[Len];
    		int handicap;
	public:
		Golf(const char * name,int hc);
		Golf();
		void sethandicap(int hc);
		void showgolf() const;
};

#endif
