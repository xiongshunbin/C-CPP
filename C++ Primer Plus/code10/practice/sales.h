#ifndef __SALES_H__
#define __SALES_H__

#include <iostream>
namespace SALES
{
	const int QUARTERS = 4;

	class Sales
	{
		private:
			double sales[QUARTERS];
			double average;
			double max;
			double min;
		public:
			Sales(const double ar[], int n);
			Sales();
			void showSales() const;
	};
}

#endif
