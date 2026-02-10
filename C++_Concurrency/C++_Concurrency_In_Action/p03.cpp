#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <stack>
#include <exception>
#include <memory>

std::mutex mtx1;
int shared_data = 100;

void use_lock()
{
	while (true)
	{
		mtx1.lock();
		shared_data++;
		std::cout << "Current thread ID = " << std::this_thread::get_id() << ", shared_data = " << shared_data << std::endl;
		mtx1.unlock();
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

void test_lock()
{
	std::thread t1(use_lock);
	std::thread t2([]() {
		while (true)
		{
			{
				std::lock_guard<std::mutex> lk_guard(mtx1);
				shared_data--;
				std::cout << "Current thread ID = " << std::this_thread::get_id() << ", shared_data = " << shared_data << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	});

	t1.join();
	t2.join();
}

template<typename T>
class threadsafe_stack1
{
private:
	std::stack<T> data;
	mutable std::mutex m;

public:
	threadsafe_stack1() { }
	threadsafe_stack1(const threadsafe_stack1& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}

	threadsafe_stack1& operator=(const threadsafe_stack1&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}

	// Notice
	T pop()
	{
		std::lock_guard<std::mutex> lock(m);
		auto element = data.top();
		data.pop();
		return element;
	}

	// Dangerous
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

void test_threadsafe_stack1()
{
	threadsafe_stack1<int> safe_stack;
	safe_stack.push(1);

	std::thread t1([&safe_stack]() {
		if (!safe_stack.empty())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			safe_stack.pop();
		}
	});

	std::thread t2([&safe_stack]() {
		if (!safe_stack.empty())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			safe_stack.pop();
		}
	});

	t1.join();
	t2.join();
}

struct empty_stack : std::exception
{
	const char* what() const throw()
	{
		return "empty stack!";
	}
};

template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;

public:
	threadsafe_stack() { }
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}

	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}

	// 使用智能指针减少返回值的拷贝
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
		{
			throw nullptr;
		}

		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	// 使用引用减少返回值的拷贝
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
		{
			throw empty_stack();
		}

		value = data.top();
		data.pop();
	}

	// Dangerous
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}

};

void test_threadsafe_stack()
{
	threadsafe_stack<int> safe_stack;
	safe_stack.push(1);

	std::thread t1([&safe_stack]() {
		if (!safe_stack.empty())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try
			{
				int value;
				safe_stack.pop(value);
			}
			catch (const empty_stack& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	});

	std::thread t2([&safe_stack]() {
		if (!safe_stack.empty())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try
			{
				int value;
				safe_stack.pop(value);
			}
			catch (const empty_stack& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	});

	t1.join();
	t2.join();
}

std::mutex t_lock1;
std::mutex t_lock2;
int m_1 = 0;
int m_2 = 1;

void dead_lock1()
{
	while (true)
	{
		std::cout << "dead_lock1 begin..." << std::endl;
		t_lock1.lock();
		m_1 = 1024;
		t_lock2.lock();
		m_2 = 2048;
		t_lock2.unlock();
		t_lock1.unlock();
		std::cout << "dead_lock1 end..." << std::endl;
	}
}

void dead_lock2()
{
	while (true)
	{
		std::cout << "dead_lock2 begin..." << std::endl;
		t_lock2.lock();
		m_2 = 2048;
		t_lock1.lock();
		m_1 = 1024;
		t_lock1.unlock();
		t_lock2.unlock();
		std::cout << "dead_lock2 end..." << std::endl;
	}
}

void test_dead_lock()
{
	std::thread t1(dead_lock1);
	std::thread t2(dead_lock2);
	t1.join();
	t2.join();
}

// 加锁和解锁作为原子操作解耦合, 各自只管理自己的功能
void atomic_lock1()
{
	std::cout << "lock1 begin lock" << std::endl;
	t_lock1.lock();
	m_1 = 1024;
	t_lock1.unlock();
	std::cout << "lock1 end lock" << std::endl;
}

void atomic_lock2()
{
	std::cout << "lock2 begin lock" << std::endl;
	t_lock2.lock();
	m_2 = 2048;
	t_lock2.unlock();
	std::cout << "lock2 end lock" << std::endl;
}

void safe_lock1()
{
	while (true)
	{
		atomic_lock1();
		atomic_lock2();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

void safe_lock2()
{
	while (true)
	{
		atomic_lock2();
		atomic_lock1();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

void test_safe_lock()
{
	std::thread t1(safe_lock1);
	std::thread t2(safe_lock2);
	t1.join();
	t2.join();
}

// 对于要使用两个互斥量, 可以同时加锁, 如不同时加锁, 可能会死锁
class some_big_object
{
public:
	some_big_object(int data) : _data(data) { }
	// 拷贝构造
	some_big_object(const some_big_object& b2) : _data(b2._data) { }
	// 移动构造
	some_big_object(some_big_object&& b2) noexcept : _data(std::move(b2._data)) { }
	// 拷贝赋值运算符重载
	some_big_object& operator=(const some_big_object& b2)
	{
		if (this != &b2)
		{
			_data = b2._data;
		}
		return *this;
	}
	// 输出运算符重载
	friend std::ostream& operator<<(std::ostream& os, const some_big_object& big_obj)
	{
		os << big_obj._data;
		return os;
	}


private:
	int _data;
};


#if 1

int main()
{
	// test_lock();

	// test_threadsafe_stack1();

	// test_threadsafe_stack();

	// test_dead_lock();

	// test_safe_lock();

	some_big_object bigobj1(100);
	some_big_object bigobj2(200);

	bigobj2 = bigobj1;

	return 0;
}

#endif
