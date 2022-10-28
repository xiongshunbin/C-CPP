// mytime2.h -- Time class after operator overloading
#ifndef MYTIME2_H__
#define MYTIME2_H__

class Time
{
	private:
		int hours;
		int minutes;
	public:
		Time();
		Time(int h,int m=0);
		void AddMin(int m);
		void AddHr(int h);
		void Reset(int h=0,int m=0);
		Time operator+(const Time & t) const;
		Time operator-(const Time & t) const;
		Time operator*(double mult) const;
		friend Time operator*(double,const Time & t);
		void Show() const;
};

// Time operator*(double m,const Time &t);

#endif
