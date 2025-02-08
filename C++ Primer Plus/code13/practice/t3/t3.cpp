// usedma.cpp -- inheritance, friends, and DMA
// compile with dma.cpp

#include <iostream>
#include "dma.h"
int main()
{
	using std::cout;
	using std::endl;
	
	baseDMA shirt("Portabelly", 8);
	lacksDMA balloon("red", "Blimpo", 4);
	hasDMA map("Buffalo Keys", 5, "Mercator");
	cout << "Displaying baseDMA object:\n";
	cout << shirt << endl;
	cout << "Displaying lackDMA object:\n";
	cout << balloon << endl;
	cout << "Displaying hasDMA object:\n";
	cout << map << endl;
	lacksDMA balloon2(balloon);
	cout << "Result of lacksDMA copy:\n";
	cout << balloon2 << endl;
	// call operator function
	hasDMA map2;
	map2 = map;
	
	// call copy construct function
	// hasDMA map2 = map;
	
	cout << "Result of hasDMA assignment:\n";
	cout << map2;
	return 0;
}