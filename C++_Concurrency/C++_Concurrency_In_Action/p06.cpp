#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <memory>

int num = 1;
std::mutex mtx_num;
std::condition_variable cvA;
std::condition_variable cvB;

void poorImplemention()
{
	std::thread t1([]() {
		while (true)
		{
			{
				std::lock_guard<std::mutex> lock(mtx_num);
				if (num == 1)
				{
					std::cout << "Thread A: " << 1 << std::endl;
					num++;
					continue;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	std::thread t2([]() {
		while (true)
		{
			{
				std::lock_guard<std::mutex> lock(mtx_num);
				if (num == 2)
				{
					std::cout << "Thread B: " << 2 << std::endl;
					num--;
					continue;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	t1.join();
	t2.join();
}

void resonableImplemention()
{
	std::thread t1([]() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(mtx_num);
			/** 
			 * 防止操作系统虚假唤醒线程
			 * 
			 * 写法1:
			 *	while(num != 1)
			 *	{
			 *		cvA.wait(lock);
			 *	}
			 */

			// 写法2 
			cvA.wait(lock, []() {
				return num == 1;
			});

			num++;
			std::cout << "Thread A: " << 1 << std::endl;
			cvB.notify_one();
		}
	});

	std::thread t2([]() {
		while (true)
		{
			std::unique_lock<std::mutex> lock(mtx_num);
			cvB.wait(lock, []() {
				return num == 2;
			});

			num--;
			std::cout << "Thread B: " << 2 << std::endl;
			cvA.notify_one();
		}
	});

	t1.join();
	t2.join();
}

// 线程安全的队列
template<typename T>
class threadsafe_queue
{
public:
	threadsafe_queue() { }
	threadsafe_queue(const threadsafe_queue& other)
	{
		std::lock_guard<std::mutex> lock(mtx);
		data_queue = other.data_queue;
	}

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(mtx);
		data_queue.push(new_value);
		data_cond.notify_one();
	}

	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lock(mtx);
		data_cond.wait(lock, [this]() {
			return !data_queue.empty();
		});

		value = data_queue.front();
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lock(mtx);
		data_cond.wait(lock, [this]() {
			return !data_queue.empty();
		});
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (data_queue.empty())
		{
			return false;
		}
		value = data_queue.front();
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (data_queue.empty())
		{
			return nullptr;
		}
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(mtx);
		return data_queue.empty();
	}

private:
	std::mutex mtx;
	std::condition_variable data_cond;
	std::queue<T> data_queue;
};

void test_safe_queue()
{
	threadsafe_queue<int> safe_queue;
	std::mutex mtx_print;
	std::thread producer([&]() {
		int i = 0;
		while (true)
		{
			safe_queue.push(i);

			{
				std::lock_guard<std::mutex> printlk(mtx_print);
				std::cout << "Producer push: " << i << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			i++;
		}
	});

	std::thread consumer1([&]() {
		while (true)
		{
			auto data = safe_queue.wait_and_pop();

			{
				std::lock_guard<std::mutex> printlk(mtx_print);
				std::cout << "Consumer1 wait and pop: " << *data << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	std::thread consumer2([&]() {
		while (true)
		{
			auto data = safe_queue.try_pop();

			if(data != nullptr)
			{
				std::lock_guard<std::mutex> printlk(mtx_print);
				std::cout << "Consumer2 try pop: " << *data << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	producer.join();
	consumer1.join();
	consumer2.join();
}

#if 0

int main()
{
	// poorImplemention();

	// resonableImplemention();

	test_safe_queue();

	return 0;
}

#endif
