// dma.cpp -- dma class methods

#include "dma.h"
#include <cstring>

// baseDMA methods
ABC::ABC(const char * l, int r)
{
	lable = new char[std::strlen(l) + 1];
	std::strcpy(lable, l);
	rating = r;
}

ABC::ABC(const ABC & rs)
{
	lable = new char[std::strlen(rs.lable) + 1];
	std::strcpy(lable, rs.lable);
	rating = rs.rating;
}

ABC::~ABC()
{
	delete [] lable;
}

ABC & ABC::operator=(const ABC & rs)
{
	if(this == &rs)
		return *this;
	delete [] lable;
	lable = new char[std::strlen(rs.lable) + 1];
	std::strcpy(lable, rs.lable);
	rating = rs.rating;
	return *this;
}

std::ostream & operator<<(std::ostream & os, const ABC & rs)
{
	os << "Lable: " << rs.lable << std::endl;
	os << "Rating: " << rs.rating << std:: endl;
	return os;
}

void ABC::View() const
{
	std::cout << "Lable: " << lable << std::endl;
	std::cout << "Rating: " << rating << std::endl; 
}

// baseDMA methods
baseDMA::baseDMA(const char * l, int r) : ABC(l, r)
{
}

baseDMA::baseDMA(const baseDMA & rs) : ABC(rs)
{
}

void baseDMA::View() const
{
	ABC::View();
}

// lacksDMA methods
lacksDMA::lacksDMA(const char * c, const char * l, int r) : ABC(l, r)
{
	std::strncpy(color, c, 39);
	color[39] = '\0';
}

lacksDMA::lacksDMA(const char * c, const ABC & rs) : ABC(rs)
{
	std::strncpy(color, c, COL_LEN - 1);
	color[COL_LEN - 1] = '\0';
}

std::ostream & operator<<(std::ostream &os, const lacksDMA & ls)
{
	os << (const ABC &) ls;
	os << "Color: " << ls.color << std::endl;
	return os;
}

void lacksDMA::View() const
{
	ABC::View();
	std::cout << "Color: " << color << std::endl;
}
// hasDMA methods
hasDMA::hasDMA(const char * l, int r, const char * s) : ABC(l, r)
{
	style = new char[std::strlen(s) + 1];
	std::strcpy(style, s);
}

hasDMA::hasDMA(const ABC & rs, const char * s) : ABC(rs)
{
	style = new char[std::strlen(s) + 1];
	std::strcpy(style, s);	
}

hasDMA::hasDMA(const hasDMA & hs) 
		: ABC(hs)	// invoke base class copy constructor
{
	style = new char[std::strlen(hs.style) + 1];
	std::strcpy(style, hs.style);
}

hasDMA::~hasDMA()
{
	delete [] style;
}

hasDMA & hasDMA::operator=(const hasDMA & hs)
{
	if(this == &hs)
		return *this;
	ABC::operator=(hs);		// copy base portion
	delete [] style;			// prepare for new style
	style = new char[std::strlen(hs.style) + 1];
	std::strcpy(style, hs.style);
	return *this;
}

std::ostream & operator<<(std::ostream & os, const hasDMA & hs)
{
	os << (const ABC &)hs;
	os << "Style: " << hs.style << std::endl;
	return os;
}

void hasDMA::View() const
{
	ABC::View();
	std::cout << "Style: " << style << std::endl;
}
