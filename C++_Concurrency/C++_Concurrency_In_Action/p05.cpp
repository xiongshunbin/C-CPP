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

// ¶öººÊ½
class Single2Hungry
{
private:
	Single2Hungry() { }
	Single2Hungry(const Single2Hungry&) = delete;
	Single2Hungry& operator=(const Single2Hungry&) = delete;

public:
	static Single2Hungry* getInstance()
	{
		if (single == nullptr)
		{
			single = new Single2Hungry;
		}
		return single;
	}

private:
	static Single2Hungry* single;
};

Single2Hungry* Single2Hungry::single = Single2Hungry::getInstance();

void thread_func_s2(int i)
{
	std::cout << "Thread[" << i << "]: " << Single2Hungry::getInstance() << std::endl;
}

void test_single2hungry()
{
	std::cout << "Single1 address: " << Single2Hungry::getInstance() << std::endl;
	std::cout << "Single2 address: " << Single2Hungry::getInstance() << std::endl;

	for (int i = 0; i < 3; ++i)
	{
		std::thread tid(thread_func_s2, i);
		tid.join();
	}
}

#if 1

int main()
{
	test_single2();
	return 0;
}

#endif
