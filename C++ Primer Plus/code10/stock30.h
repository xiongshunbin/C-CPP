// stock30.h
#ifndef STOCK30_H__
#define STOCK30_h__
#include <string>

class Stock
{
    private:
        std::string company;
        long shares;
        double share_val;
        double total_val;
        void set_tot(){total_val=shares*share_val;}
    public:
	// two constructors
//	Stock();// default constructor
//	Stock(const std::string & co,long n=0,double pr=0.0);
	Stock(const std::string & co="Error",long n=0,double pr=0.0);
//	~Stock();// do-nothing destructor
	void buy(long num,double price);
        void sell(long num,double price);
        void update(double price);
	void show() const;
        const Stock & topval(const Stock & s) const;
	const std::string & co_name() const {return company;}
	long numshares() const {return shares;}
	double shareval() const {return share_val;}
	double totalval() const {return total_val;}
};

#endif
