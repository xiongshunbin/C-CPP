// stonewt.h -- definition for the Stonewt class
#ifndef STONEWT_H__
#define STONEWT_H__

#include <iostream>

class Stonewt
{
	public:
		enum  Type{STONE,POUNDS,FLOATPOUNDS};
	private:
		enum {Lbs_per_stn=14};	// pounds per stone
		int stone;		// whole stones
		double pds_left;	// fractional pounds
		double pounds;		// entire weight in pounds
		Type type;
	public:
//		explicit Stonewt(double lbs);	// constructor for double pounds
		Stonewt(double lbs);
		Stonewt(int stn,double lbs);	// constructor for stone, lbs
		Stonewt();	//default constructor
		~Stonewt();

		void set_type(Type m);
		Stonewt operator+(const Stonewt & s) const;
		Stonewt operator-(const Stonewt & s) const;
		Stonewt operator*(double n) const;

		friend std::ostream & operator<<(std::ostream & os,const Stonewt & s);
};

#endif
