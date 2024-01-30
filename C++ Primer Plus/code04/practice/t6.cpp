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
	CandyBar snack[3]=
	{
		{"Mocha Munch",2.3,350},
		{"Flip Fleming",3.2,500},
		{"Micheal Bay",4.3,400}
	};
	cout<<snack[0].brand<<" weight: "<<snack[0].weight
		<<" calorie "<<snack[0].calorie<<endl;
	cout<<snack[1].brand<<" weight: "<<snack[1].weight
		<<" calorie "<<snack[1].calorie<<endl;
	cout<<snack[2].brand<<" weight: "<<snack[2].weight
		<<" calorie "<<snack[2].calorie<<endl;	
	
	
	
	return 0;
}
