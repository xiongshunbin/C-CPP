#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <map>

// unique_lock 基本用法
std::mutex mtx;
int shared_data = 0;
void use_unique()
{
	// unique_lock 可以自动析构解锁, 也可以手动解锁
	std::unique_lock<std::mutex> lock(mtx);
	std::cout << "lock success!" << std::endl;
	shared_data++;
	lock.unlock();
}

// 可判断是否占有锁
void owns_lock()
{
	std::unique_lock<std::mutex> lock(mtx);
	shared_data++;
	if (lock.owns_lock())
	{
		std::cout << "owns lock!" << std::endl;
	}
	else
	{
		std::cout << "doesn't own lock!" << std::endl;
	}

	lock.unlock();
	if (lock.owns_lock())
	{
		std::cout << "owns lock!" << std::endl;
	}
	else
	{
		std::cout << "doesn't own lock!" << std::endl;
	}
}

// 可以延迟加锁
void defer_lock()
{
	// 延迟加锁
	std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
	lock.lock();
	lock.unlock();
}

void use_own_defer()
{
	std::unique_lock<std::mutex> lock(mtx);
	if (lock.owns_lock())
	{
		std::cout << "Main thread has the lock!" << std::endl;
	}
	else
	{
		std::cout << "Main thread doesn't have the lock!" << std::endl;
	}

	std::thread t([]() {
		std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
		if (lock.owns_lock())
		{
			std::cout << "Thread has the lock!" << std::endl;
		}
		else
		{
			std::cout << "Thread doesn't have the lock!" << std::endl;
		}

		lock.lock();

		if (lock.owns_lock())
		{
			std::cout << "Thread has the lock!" << std::endl;
		}
		else
		{
			std::cout << "Thread doesn't have the lock!" << std::endl;
		}

		lock.unlock();
	});

	t.join();
}

// 同样支持领养操作
void use_own_adopt()
{
	mtx.lock();
	std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
	if (lock.owns_lock())
	{
		std::cout << "owns lock!" << std::endl;
	}
	else
	{
		std::cout << "doesn't own lock!" << std::endl;
	}
	lock.unlock();
}

int a = 10;
int b = 99;
std::mutex mtx1;
std::mutex mtx2;

void safe_swap()
{
	std::lock(mtx1, mtx2);
	std::unique_lock<std::mutex> lock1(mtx1, std::adopt_lock);
	std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);
	std::swap(a, b);
	lock1.unlock();
	lock2.unlock();

	/**
	* mtx1.unlock();		// invalid
	* mtx2.unlock();		// invalid
	*/
}

void safe_swap2()
{
	std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
	std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
	std::lock(lock1, lock2);
	// std::lock(mtx1, mtx2);		// invalid
	std::swap(a, b);
}

// 转移互斥量所有权
// 互斥量本身不支持 move 操作, 但是 unique_lock 支持
std::unique_lock<std::mutex> get_lock()
{
	std::unique_lock<std::mutex> lock(mtx);
	shared_data++;
	return lock;
}

void use_return()
{
	std::unique_lock<std::mutex> lock(get_lock());
	shared_data++;
}

/**
 * 锁粒度表示加锁的(范围)精细程度
 *		1.一个锁的粒度要足够大, 以保证可以锁住要访问的共享数据
 *		2.一个锁的粒度要足够小, 以保证非共享的数据不被锁住影响性能
 */
void precision_lock()
{
	std::unique_lock<std::mutex> lock(mtx);
	shared_data++;
	lock.unlock();
	// 不涉及共享数据的耗时操作不要放在锁内执行
	std::this_thread::sleep_for(std::chrono::seconds(1));
	lock.lock();
	shared_data++;
}

/**
 * C++17 -> shared_mutex
 * C++14 -> shared_time_mutex
 * C++11 -> boost
 */ 
class DBService
{
public:
	DBService() { }
	// 读操作采用共享锁
	std::string QueryDNS(std::string dns_name)
	{
		std::shared_lock<std::shared_mutex> shared_locks(_shared_mtx);
		auto iter = _dns_info.find(dns_name);
		if (iter != _dns_info.end())
		{
			return iter->second;
		}

		return "";
	}

	// 写操作采用独占锁
	void AddDNSInfo(std::string dns_name, std::string dns_entry)
	{
		std::lock_guard<std::shared_mutex> guard_locks(_shared_mtx);
		_dns_info.insert(std::make_pair(dns_name, dns_entry));
	}

private:
	std::map<std::string, std::string> _dns_info;
	mutable std::shared_mutex _shared_mtx;
};

// 递归锁
class RecursiveDemo
{
public:
	RecursiveDemo() { }
	bool QueryStudent(std::string name)
	{
		std::lock_guard<std::recursive_mutex> recursive_lock(_recursive_mtx);
		auto iter_find = _student_info.find(name);
		if (iter_find == _student_info.end())
		{
			return false;
		}
		return true;
	}

	void AddScore(std::string name, int score)
	{
		std::lock_guard<std::recursive_mutex> recursive_lock(_recursive_mtx);
		if (!QueryStudent(name))
		{
			_student_info.insert(std::make_pair(name, score));
			return;
		}
		_student_info[name] += score;
	}

	// 不推荐采用用递归锁, 可通过拆分逻辑的方式优化设计, 将共有逻辑拆分为统一接口
	void AddScoreAtomic(std::string name, int score)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		auto iter_find = _student_info.find(name);
		if (iter_find == _student_info.end())
		{
			_student_info.insert(std::make_pair(name, score));
			return;
		}

		_student_info[name] += score;
	}

private:
	std::map<std::string, int> _student_info;
	std::mutex _mtx;
	std::recursive_mutex _recursive_mtx;
};

#if 1

int main()
{
	// use_unique();

	// owns_lock();

	// defer_lock();

	// use_own_defer();

	// use_own_adopt();

	// safe_swap();

	// safe_swap2();

	use_return();

	return 0;
}

#endif