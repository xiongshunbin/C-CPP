// stock.h -- augmented version
#ifndef STOCK_H__
#define STOCK_h__
#include <cstring>

class Stock
{
    private:
        char *company;
        long shares;
        double share_val;
        double total_val;
        void set_tot(){total_val=shares*share_val;}
    public:
	// two constructors
	Stock();// default constructor
	Stock(const char *co,long n=0,double pr=0.0);
//	Stock(const std::string & co="Error",long n=0,double pr=0.0);
	~Stock();// do-nothing destructor
	void buy(long num,double price);
        void sell(long num,double price);
        void update(double price);
        const Stock & topval(const Stock & s) const;
	friend std::ostream & operator<<(std::ostream &os, const Stock &s);
};

#endif
