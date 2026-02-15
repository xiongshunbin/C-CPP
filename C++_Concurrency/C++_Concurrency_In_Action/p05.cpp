#include <iostream>
#include <thread>
#include <mutex>

class Single2
{
private:
	Single2() { }
	Single2(const Single2&) = delete;
	Single2& operator=(const Single2&) = delete;

public:
	static Single2& getInstance()
	{
		static Single2 single;
		return single;
	}
};

void test_single2()
{
	std::cout << "single1 address: " << &Single2::getInstance() << std::endl;
	std::cout << "single2 address: " << &Single2::getInstance() << std::endl;
}

#if 1

int main()
{
	test_single2();
	return 0;
}

#endif
