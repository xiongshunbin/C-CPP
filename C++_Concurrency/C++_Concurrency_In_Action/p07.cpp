#include <iostream>
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

#if 1

int main()
{
	use_async();

	return 0;
}

#endif
