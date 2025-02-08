// stonewt.cpp -- Stonewt methods
#include "stonewt.h"
using std::cout;

// construct Stonewt object from double value
Stonewt::Stonewt(double lbs)
{
	stone=(int) lbs/Lbs_per_stn;	// integer division
	pds_left=int(lbs)%Lbs_per_stn+lbs-int(lbs);
	pounds=lbs;
	type=FLOATPOUNDS;
}
// construct Stonewt object from stone, double values
// if provide this constructor second argument with a default value
// it will be used which equlse an integer
Stonewt::Stonewt(int stn,double lbs)
{
	stone=stn;
	pds_left=lbs;
	pounds=stn*Lbs_per_stn+lbs;
	type=STONE;
}

Stonewt::Stonewt()	// default constuctor, wt = 0
{
	stone=pounds=pds_left=0;
	type=POUNDS;
}

Stonewt::~Stonewt()	// destructor
{
}

void Stonewt::set_type(Type m)
{
	type=m;
}

Stonewt Stonewt::operator+(const Stonewt & s) const
{
	return Stonewt(pounds+s.pounds);
}
Stonewt Stonewt::operator-(const Stonewt & s) const
{
	return Stonewt(pounds-s.pounds);
}
Stonewt Stonewt::operator*(double n) const
{
	return (pounds*n);
}

std::ostream & operator<<(std::ostream & os,const Stonewt & s)
{
	if(s.type==Stonewt::STONE)
		os<<s.stone<<" stone, "<<s.pds_left<<" pounds.\n";
	else if(s.type==Stonewt::POUNDS)
		os<<(int)(s.pounds+0.5)<<" pounds.\n";
	else if(s.type==Stonewt::FLOATPOUNDS)
		os<<s.pounds<<" pounds.\n";
	else
		os<<"Invalid type.\n";
	return os;
}
