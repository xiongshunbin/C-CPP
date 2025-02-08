// stonewt3.cpp -- Stonewt methods
#include <iostream>
using std::cout;
#include "stonewt3.h"

// construct Stonewt object from double value
Stonewt::Stonewt(double lbs)
{
	stone=(int) lbs/Lbs_per_stn;	// integer division
	pds_left=int(lbs)%Lbs_per_stn+lbs-int(lbs);
	pounds=lbs;
}
// construct Stonewt object from stone, double values
// if provide this constructor second argument with a default value
// it will be used which equlse an integer
Stonewt::Stonewt(int stn,double lbs)
{
	stone=stn;
	pds_left=lbs;
	pounds=stn*Lbs_per_stn+lbs;
}

Stonewt::Stonewt()	// default constuctor, wt = 0
{
	stone=pounds=pds_left=0;
}

Stonewt::~Stonewt()	// destructor
{
}
// show weight in stones
void Stonewt::show_stn() const
{
	cout<<stone<<" stone, "<<pds_left<<" pounds\n";
}
// show weight in pounds
void Stonewt::show_lbs() const
{
	cout<<pounds<<" pounds\n";
}
Stonewt Stonewt::operator*(double mult) const
{
	return Stonewt(pounds*mult);
}

Stonewt operator*(double mult,const Stonewt & st)
{
	return Stonewt(mult*st.pounds);
}
