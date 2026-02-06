#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>

void some_function()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void some_other_function()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/**
 * std::thread 不支持拷贝构造和拷贝赋值运算符, 只支持移动构造和移动赋值运算符;
 * 不要将一个线程的管理权交给一个已经绑定线程的变量, 否则会触发线程的 terminate 函数引发崩溃。
 */
void dangerous_use()
{
	// t1 绑定 some_function
	std::thread t1(some_function);
	// 将 t1 的线程所有权转移给 t2
	std::thread t2 = std::move(t1);
	// t1 可继续绑定其他线程, 执行 some_other_function
	t1 = std::thread(some_other_function);
	std::thread t3;
	// 将 t2 的线程所有权转移给 t3
	t3 = std::move(t2);
	// 将 t3 的线程所有权转移给 t1
	t1 = std::move(t3);		// 可能导致崩溃
	std::this_thread::sleep_for(std::chrono::seconds(2000));
}

// 返回局部的 std::thread 对象时, 调用的是 sth::thread 的移动构造函数
std::thread f()
{
	return std::thread(some_function);
}

void param_function(int a)
{
	while (true)
	{
		std::cout << "Param is " << a << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

std::thread g()
{
	std::thread t(param_function, 43);
	return t;
}

class joining_thread
{
private:
	std::thread _t;
public:
	joining_thread() noexcept = default;
	template<typename Callable, typename... Args>
	explicit joining_thread(Callable&& func, Args&&... args) :
		_t(std::forward<Callable>(func), std::forward<Args>(args)...) { }
	explicit joining_thread(std::thread t) noexcept : _t(std::move(t)) { }
	joining_thread(joining_thread&& other) noexcept : _t(std::move(other._t))
	{
		std::cout << "use move constructor!";
	}

	joining_thread& operator=(joining_thread&& other) noexcept
	{
		// 如果当前线程可汇合, 则汇合等待线程完成再赋值
		if (joinable())
		{
			join();
		}
		_t = std::move(other._t);
		return *this;
	}

	joining_thread& operator=(std::thread other) noexcept
	{
		// 如果当前线程可汇合, 则汇合等待线程完成再赋值
		if (joinable())
		{
			join();
		}
		_t = std::move(other);
		return *this;
	}

	~joining_thread() noexcept
	{
		if (joinable())
		{
			join();
		}
	}

	void swap(joining_thread& other) noexcept
	{
		_t.swap(other._t);
	}

	std::thread::id get_id() const noexcept
	{
		return _t.get_id();
	}

	bool joinable() const noexcept
	{
		return _t.joinable();
	}

	void join()
	{
		_t.join();
	}

	void detach()
	{
		_t.detach();
	}

	std::thread& as_thread() noexcept
	{
		return _t;
	}

	const std::thread& as_thread() const noexcept
	{
		return _t;
	}
};

void use_jointhread()
{
	// 根据线程构造函数构造 joining_thread
	joining_thread j1([](int max_index) {
		for (int i = 0; i < max_index; i++)
		{
			std::cout << "The thread (id: " << std::this_thread::get_id() << ") current index = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}, 10);

	// 根据 thread 构造 joining_thread
	joining_thread j2(std::thread([](int max_index) {
		for (int i = 0; i < max_index; i++)
		{
			std::cout << "The thread (id: " << std::this_thread::get_id() << ") current index = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}, 10));

	// 把 j2 赋值给 j1 时, joining_thread 内部会等待 j1 汇合结束后再将 j2 赋值给 j1
	// j1 = j2;		// invalid
	j1 = std::move(j2);
}

void use_vector()
{
	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 10; ++i)
	{
		/**
		 * 		std::thread t = std::thread(param_function, i);
		 *		threads.push_back(std::move(t));
		 */
		threads.emplace_back(param_function, i);
	}

	for (auto& entry : threads)
	{
		entry.join();
	}
}

// 并行版的 std::accumulate() 的简单实现
template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first, last, result);
	}
};
template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return init;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end,std::ref(results[i]));
		block_start = block_end;
	}
	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
	for (auto& entry : threads)
		entry.join();
	return std::accumulate(results.begin(), results.end(), init);
}

void use_parallel_acc()
{
	std::vector<int> vec(10000);
	for (int i = 0; i < 10000; ++i)
	{
		vec.push_back(i);
	}

	long sum = 0;
	sum = parallel_accumulate<std::vector<int>::iterator, long>(vec.begin(), vec.end(), sum);
	std::cout << "Sum = " << sum << std::endl;
}

#if 0

int main()
{
	// dangerous_use();

	//use_jointhread();

	//use_vector();

	use_parallel_acc();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}

#endif
