#include <iostream>
#include "cow.h"

using namespace std;

int main()
{
	Cow cow1;
	Cow cow2("marry", "badminton", 237.5);
	Cow cow3(cow2);
	cow1 = cow2;

	cow1.ShowCow();
	cow2.ShowCow();
	cow3.ShowCow();
	return 0;
}
