#include<iostream>
using namespace std;

struct box
{
	char maker[40];
	float height;
	float width;
	float length;
	float volume;
};

void set(box * b);
void show(box b);

int main()
{
	box b={"cube",3,4,5};
	set(&b);
	show(b);
	
	return 0;
}

void set(box *pb)
{
	pb->volume=pb->height*pb->width*pb->length;
}
void show(box b)
{
	cout<<"Maker: "<<b.maker<<endl
		<<"Height:"<<b.height<<endl
		<<"Width: "<<b.width<<endl
		<<"Length: "<<b.length<<endl
		<<"Volume: "<<b.volume<<endl;
		
}
