#include<iostream>

using namespace std;
struct CandyBar
{
	char brand[20];
	float weight;
	unsigned int calorie;
};
int main()
{
	CandyBar* pt=new CandyBar[3];
	
	pt[0]={"Mocha Munch",2.3,350};
	pt[1]={"Flip Fleming",3.2,500};
	pt[2]={"Micheal Bay",4.3,400};
	
	cout<<pt->brand<<" weight: "<<pt->weight
	<<" calorie "<<pt->calorie<<endl;
	cout<<(pt+1)->brand<<" weight: "<<(pt+1)->weight
	<<" calorie "<<(pt+1)->calorie<<endl;
	cout<<(pt+2)->brand<<" weight: "<<(pt+2)->weight
	<<" calorie "<<(pt+1)->calorie<<endl;	
	
	
	
	return 0;
}
