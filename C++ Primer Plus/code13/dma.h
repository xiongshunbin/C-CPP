// dma.h -- inheritance and dynamic memory allocation
#ifndef DMA_H_
#define DMA_H_
#include <iostream>

// Base Class Using DMA
class baseDMA
{
	private:
		char * lable;
		int rating;

	public:
	baseDMA(const char * l = "null", int r = 0);
	baseDMA(const baseDMA & rs);
	virtual ~baseDMA();
	baseDMA & operator=(const baseDMA & rs);
	friend std::ostream & operator<<(std::ostream &os,
					const baseDMA & rs);
};

// derived class without DMA
// no destructor needed
// uses implicit copy constructor
// uses implicit assignment operator
class lacksDMA : public baseDMA
{
	private:
		enum { COL_LEN = 40 };
		char color[COL_LEN];
	public:
		lacksDMA(const char * c = "blank", const char * l = "null",
					int r = 0);
		lacksDMA(const char * c, const baseDMA & rs);
		friend std::ostream & operator<<(std::ostream &os,
					const lacksDMA & ls);
};

// derived class with DMA
class hasDMA : public baseDMA
{
	private:
			char * style;
	public:
			hasDMA(const char * l = "null", int r = 0, const char * s = "none");
			hasDMA(const baseDMA & rs, const char * s);
			hasDMA(const hasDMA & hs);
			~hasDMA();
			hasDMA & operator=(const hasDMA & hs);
			friend std::ostream & operator<<(std::ostream & os, const hasDMA & hs);
};
#endif
