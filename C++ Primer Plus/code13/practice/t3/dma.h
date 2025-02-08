// dma.h -- inheritance and dynamic memory allocation
#ifndef DMA_H_
#define DMA_H_
#include <iostream>

class ABC
{
	private:
		char * lable;
		int rating;
	public:
	ABC(const char * l = "null", int r = 0);
	ABC(const  ABC& rs);
	virtual ~ABC();
	ABC & operator=(const  ABC& rs);
	virtual void View() const;
	friend std::ostream & operator<<(std::ostream &os,
					const ABC & rs);
};

// Base Class Using DMA
class baseDMA : public ABC
{
	public:
		baseDMA(const char * l = "null", int r = 0);
		baseDMA(const baseDMA & rs);
		virtual void View() const;
};

// derived class without DMA
// no destructor needed
// uses implicit copy constructor
// uses implicit assignment operator
class lacksDMA : public ABC
{
	private:
		enum { COL_LEN = 40 };
		char color[COL_LEN];
	public:
		lacksDMA(const char * c = "blank", const char * l = "null",
					int r = 0);
		lacksDMA(const char * c, const ABC & rs);
		friend std::ostream & operator<<(std::ostream &os,
					const lacksDMA & ls);
		virtual void View() const;
};

// derived class with DMA
class hasDMA : public ABC
{
	private:
			char * style;
	public:
			hasDMA(const char * l = "null", int r = 0, const char * s = "none");
			hasDMA(const ABC & rs, const char * s);
			hasDMA(const hasDMA & hs);
			~hasDMA();
			hasDMA & operator=(const hasDMA & hs);
			friend std::ostream & operator<<(std::ostream & os, const hasDMA & hs);
			virtual void View() const;
};
#endif
