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
				std::cout << "stack is empty!" << std::endl;
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

#if 1

int main()
{
	// test_lock();

	// test_threadsafe_stack1();

	test_threadsafe_stack();

	return 0;
}

#endif
