#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <string>

// 定义一个异步任务
std::string fetchDataFromDB(std::string query)
{
	// 模拟一个异步任务, 比如从数据库中获取数据
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return "Data: " + query;
}

void use_async()
{
	// 使用 std::async 异步调用 fetchDataFromDB
	std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "data");

	// 在主线程中做其他事情
	std::cout << "Doing other things..." << std::endl;

	// 从 future 对象中获取数据
	std::string dbData = resultFromDB.get();
	std::cout << dbData << std::endl;
}

int my_task()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "My task run 5 s." << std::endl;
	return 42;
}

void use_package()
{
	// 创建一个包装了任务的 std::packaged_task 对象
	std::packaged_task<int()> task(my_task);

	// 获取与任务关联的 std::future 对象
	std::future<int> result = task.get_future();

	// 在另一个线程上执行任务
	std::thread t(std::move(task));
	t.detach();		// 将线程与主线程分离, 以便主线程可以等待任务完成

	// 等待任务完成并获取结果
	int value = result.get();
	std::cout << "The thread task result: " << value << std::endl;
}

void set_value(std::promise<int> prom)
{
	// 设置 promise 的值
	std::this_thread::sleep_for(std::chrono::seconds(5));
	prom.set_value(10);
	std::cout << "Promise set value success!" << std::endl;
}

void use_promise()
{
	// 创建一个 promise 对象
	std::promise<int> prom;

	// 获取与 promise 相关的 future 对象
	std::future<int> fut = prom.get_future();

	// 在新线程中设置 promise 的值
	std::thread t(set_value, std::move(prom));

	// 在主线程中获取 future 的值
	std::cout << "Waiting for the thread to set the value." << std::endl;
	std::cout << "Value set by the thread: " << fut.get() << std::endl;
	t.join();
}

void set_exception(std::promise<int> prom)
{
	try
	{
		// 抛出一个异常
		throw std::runtime_error("An error occurred!");
	}
	catch (...)
	{
		// 设置 promise 的异常
		prom.set_exception(std::current_exception());
	}
}

void use_promise_exception()
{

}

#if 1

int main()
{
	// use_async();

	// use_package();

	use_promise();

	return 0;
}

#endif
