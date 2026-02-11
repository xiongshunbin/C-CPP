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
	some_big_object& operator=(some_big_object&& b2) noexcept
	{
		_data = std::move(b2._data);
		return *this;
	}
	// 输出运算符重载
	friend std::ostream& operator<<(std::ostream& os, const some_big_object& big_obj)
	{
		os << big_obj._data;
		return os;
	}

	// 交换数据
	friend void swap(some_big_object& b1, some_big_object& b2);

private:
	int _data;
};

void swap(some_big_object& b1, some_big_object& b2)
{
	some_big_object temp = std::move(b1);
	b1 = std::move(b2);
	b2 = std::move(temp);
}

class big_object_mgr
{
public:
	big_object_mgr(int data = 0) : _obj(data) { }
	void printinfo()
	{
		std::cout << "Current obj data is: " << _obj << std::endl;
	}

	friend void danger_swap(big_object_mgr& objm1, big_object_mgr& objm2);
	friend void safe_swap(big_object_mgr& objm1, big_object_mgr& objm2);
	friend void safe_swap_scope(big_object_mgr& objm1, big_object_mgr& objm2);

private:
	std::mutex _mtx;
	some_big_object _obj;
};

void danger_swap(big_object_mgr& objm1, big_object_mgr& objm2)
{
	std::cout << "Thread [" << std::this_thread::get_id() << "] begin." << std::endl;
	if (&objm1 == &objm2)
	{
		return;
	}

	std::lock_guard<std::mutex> guard1(objm1._mtx);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::lock_guard<std::mutex> guard2(objm2._mtx);
	swap(objm1._obj, objm2._obj);
	std::cout << "Thread [" << std::this_thread::get_id() << "] end." << std::endl;
}

void test_danger_swap()
{
	big_object_mgr objm1(5);
	big_object_mgr objm2(100);

	std::thread t1(danger_swap, std::ref(objm1), std::ref(objm2));
	std::thread t2(danger_swap, std::ref(objm2), std::ref(objm1));

	t1.join();
	t2.join();

	objm1.printinfo();
	objm2.printinfo();
}

void safe_swap(big_object_mgr& objm1, big_object_mgr& objm2)
{
	std::cout << "Thread [" << std::this_thread::get_id() << "] begin." << std::endl;
	if (&objm1 == &objm2)
	{
		return;
	}

	std::lock(objm1._mtx, objm2._mtx);
	// 领养锁管理互斥量解锁
	std::lock_guard<std::mutex> guard1(objm1._mtx, std::adopt_lock);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::lock_guard<std::mutex> guard2(objm2._mtx, std::adopt_lock);
	swap(objm1._obj, objm2._obj);
	std::cout << "Thread [" << std::this_thread::get_id() << "] end." << std::endl;
}

void test_safe_swap()
{
	big_object_mgr objm1(5);
	big_object_mgr objm2(100);

	std::thread t1(safe_swap, std::ref(objm1), std::ref(objm2));
	std::thread t2(safe_swap, std::ref(objm2), std::ref(objm1));

	t1.join();
	t2.join();

	objm1.printinfo();
	objm2.printinfo();
}

void safe_swap_scope(big_object_mgr& objm1, big_object_mgr& objm2)
{
	std::cout << "Thread [" << std::this_thread::get_id() << "] begin." << std::endl;
	if (&objm1 == &objm2)
	{
		return;
	}

	std::scoped_lock guard(objm1._mtx, objm2._mtx);
	swap(objm1._obj, objm2._obj);
	std::cout << "Thread [" << std::this_thread::get_id() << "] end." << std::endl;
}

void test_safe_swap_scope()
{
	big_object_mgr objm1(5);
	big_object_mgr objm2(100);

	std::thread t1(safe_swap_scope, std::ref(objm1), std::ref(objm2));
	std::thread t2(safe_swap_scope, std::ref(objm2), std::ref(objm1));

	t1.join();
	t2.join();

	objm1.printinfo();
	objm2.printinfo();
}

// 层级锁
class hierarchical_mutex
{
public:
	explicit hierarchical_mutex(unsigned long value) : _hierarchy_value(value),
		_previous_hierarchy_value(0) { }

	hierarchical_mutex(const hierarchical_mutex&) = delete;
	hierarchical_mutex& operator=(const hierarchical_mutex&) = delete;

	void lock()
	{
		check_for_hierarchy_violation();
		_internal_mutex.lock();

	}

private:
	std::mutex _internal_mutex;
	unsigned long const _hierarchy_value;			// 当前层级值
	unsigned long const _previous_hierarchy_value;	// 上一层层级值
	static thread_local unsigned long const _this_thread_hierarchy_value;	// 本线程层级值
};

#if 1

int main()
{
	// test_lock();

	// test_threadsafe_stack1();

	// test_threadsafe_stack();

	// test_dead_lock();

	// test_safe_lock();

	// test_danger_swap();

	// test_safe_swap();

	test_safe_swap_scope();

	return 0;
}

#endif
