#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>

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

// ÀÁººÊ½
class SingletonOnce
{
private:
	SingletonOnce() = default;
	SingletonOnce(const SingletonOnce&) = delete;
	SingletonOnce& operator=(const SingletonOnce&) = delete;

public:
	static std::shared_ptr<SingletonOnce> getInstance()
	{
		static std::once_flag flag;
		std::call_once(flag, [&]() {

			_instance = std::shared_ptr<SingletonOnce>(new SingletonOnce);
		});
		return _instance;
	}

	void printAddress()
	{
		std::cout << _instance.get() << std::endl;
	}

	~SingletonOnce()
	{
		std::cout << "SingletonOnce destructed!" << std::endl;
	}

private:
	static std::shared_ptr<SingletonOnce> _instance;
};

std::shared_ptr<SingletonOnce> SingletonOnce::_instance = nullptr;

void TestSingle()
{
	std::thread t1([]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		SingletonOnce::getInstance()->printAddress();
	});

	std::thread t2([]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		SingletonOnce::getInstance()->printAddress();
	});

	t1.join();
	t2.join();
}

template<typename T>


#if 1

int main()
{
	// test_single2();

	// test_single2hungry();

	TestSingle();

	return 0;
}

#endif
