// myfirst.cpp -- displays a message

#include<iostream>							//a PREPROCESSOR directive
int main() {								//function header
	//start of function body
	using namespace std;					//make definitions visible
	cout << "Come up and C++ me some time.";	//message
	cout << endl;								//start a new line
	cout << "You won't regret it!" << endl;		//more output
	cout << "Behold the Beans of Beauty!";	//不能把回车放在字符串中间
	return 0;								//terminate main()
}
