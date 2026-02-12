#include <iostream>
#include <thread>
#include <mutex>

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

#if 1

int main()
{
	// use_unique();

	// owns_lock();

	// defer_lock();

	// use_own_defer();

	use_own_adopt();

	return 0;
}

#endif