#ifndef BOP_H
#define BOP_H

#include <iostream>
using namespace std;
class Port
{
    private:
        char * brand;
        char style[20]; //i.e., tawny, ruby, vintage
        int bottles;
    public:
        Port(const char * br = "none", const char * st = "none", int b = 0);
        Port(const Port & p);   // copy constructor
        virtual ~Port() { delete [] brand; }
        Port & operator=(const Port &);
        Port & operator+=(int b);   // adds b to bottles, if available
        Port & operator-=(int b);   // subtracts b from bottles, if available
        int BottleCount() const { return bottles; }
        virtual void Show() const;
        friend ostream & operator<<(ostream & os, const Port & p);
};

class VintageaPort : public Port
{
    private:
        char * nickname;    // i.e., "The Noble" or "Old Velvet", etc.
        int year;
    public:
        VintageaPort();
        VintageaPort(const char * br, const char * st, int b, const char * nn, int y);
        VintageaPort(const VintageaPort & vp);
        ~VintageaPort() { delete [] nickname; }
        VintageaPort & operator=(const VintageaPort & vp);
        void Show() const;
        friend ostream & operator<<(ostream & os, const VintageaPort & vp);
};

#endif