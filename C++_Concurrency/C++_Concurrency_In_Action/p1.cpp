#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void thread_work1(std::string str)
{
	std::cout << "String is: " << str << std::endl;
}

class background_task
{
public:
	void operator()()
	{
		std::cout << "background_task is called!" << std::endl;
	}
};

struct func
{
	int& _i;
	func(int& i) : _i(i) { }
	void operator()()
	{
		for (int i = 0; i < 3; i++)
		{
			_i = i;
			std::cout << "_i = " << _i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
};

/**
 * 线程允许采用 detach 分离的方式在后台独自运行, 这种分离的线程被称为守护线程。
 * 
 * 意图在函数中创建线程, 并让线程访问函数的局部变量。除非线程肯定会在该函数退出前结束, 否则切勿这么做, 会导致未定义行为。
 * 
 * 解决措施:
 *		1) 伪闭包策略。通过共享型智能指针 shared_ptr 传递参数, 因为引用计数会随着赋值增加, 可保证局部变量在使用期间不被释放。
 *		2) 按值传递。将局部变量的值作为参数传递, 这么做需要局部变量有拷贝复制的功能(重写拷贝构造函数), 而且拷贝耗费空间和效率。
 *		3) 将线程运行的方式修改为 join, 这样能保证局部变量被释放前线程已经运行结束, 但是这么做可能会影响运行逻辑。
 */
void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread functhread(my_func);
	// 隐患, 在子线程中访问局部变量的地址或引用, 局部变量可能会随着 } 结束而回收或随着主线程退出而回收。
	functhread.detach();
}

void use_join()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread functhread(my_func);
	functhread.join();
}

/**
 * 当子线程在执行重要的核心任务时, 主线程如果出现异常需要保证子线程稳定运行结束后, 主线程再抛出异常结束运行。
 */
void catch_exception()
{
	int some_local_state = 0;
	func myfunc(some_local_state);
	std::thread functhread{ myfunc };
	try
	{
		// 本线程做一些事情
		throw "Divide by zero exception.";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	catch (const char* s)
	{
		std::cout << s << std::endl;
		functhread.join();
		return;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception occurred! Error: " << e.what() << std::endl;
		functhread.join();
		throw;
	}

	if (functhread.joinable())
	{
		functhread.join();
	}
}

// 使用RAII技术, 保证线程对象析构的时候等待线程运行结束, 回收资源
class thread_guard
{
private:
	std::thread& _t;

public:
	explicit thread_guard(std::thread& t) : _t(t) { }
	~thread_guard()
	{
		// join() 只能调用一次
		if (_t.joinable())
		{
			_t.join();
		}
	}

	thread_guard(const thread_guard&) = delete;
	thread_guard& operator=(const thread_guard&) = delete;
};

void auto_guard()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread t(my_func);
	thread_guard g(t);
	// 本线程做一些事情
	std::cout << "Auto guard finished!" << std::endl;
}

void print_str(int i, const std::string& s)
{
	std::cout << "i = " << i << ", string: " << s << std::endl;
}

/**
 * char* const: 指针常量, 指针所指向的地址(指针自身)不能改变
 * const char*: 常量指针是指向常量的指针, 指针指向的内容不能改变
 * 
 * C++ 隐式转换在线程的调用上可能会造成崩溃问题
 */
void danger_oops(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
	// char* -> std::string
	std::thread t(print_str, 3, buffer);
	t.detach();
	std::cout << "Danger oops finished!" << std::endl;
}

#if 1

int main()
{
	std::string hellostr = "Hello world!";
	// 通过函数名初始化并启动一个线程
	std::thread t1(thread_work1, hellostr);
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	
	/**
	 * 在线程销毁前要对其调用 join 等待线程退出或 detach 将线程分离,
	 * 否则 std::thread 的析构函数会调用 std::terminate 终止程序,
	 * 注意分离线程可能出现空悬引用的隐患
	 */
	// 主线程等待子线程退出
	t1.join();

	/** C++ 最麻烦的解释(C++'s most vexing parse)
	* 
	* 	std::thread t2(background_task());
	*	t2.join();		// error
	* 
	* C++ 编译器会将 t2 解析为 std::thread (*)(background_task (*)()) 类型的函数指针变量。
	* 
	* 注: 当线程对象构造调用临时函数对象的有参构造函数时, 编译器能够正常解析。
	* 
	* 解决办法:
	*	1) 参数使用具名变量的仿函数(函数对象), 而不是临时变量
	*	eg:
	*		background_task f;
	*		std::thread t2(f);
	*		t2.join();
	* 
	*	2) 多用一对圆括号
	*	eg:
	*		std::thread t2((background_task()));
	*		t2.join();
	* 
	*	3) 采用新式的统一初始化语法(uniform initialization syntax, 又名列表初始化）
	*	eg:
	*		std::thread t2{ background_task() };
	*		t2.join();
	*/

	// 通过仿函数初始化并启动一个线程
	std::thread t2{ background_task() };
	t2.join();

	// 通过 lambda 表达式初始化并启动一个线程
	std::thread t3([](std::string str) {
		std::cout << "String is: " << str << std::endl;
	}, hellostr);
	t3.join();

	// detach 隐患
	oops();
	// 防止主线程退出过快, 需要停顿一下, 让子线程跑起来
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// 使用 join 解决子线程访问局部变量的隐患问题
	 use_join();

	 // 捕获异常
	 catch_exception();

	 // 自动守卫
	 auto_guard();

	 // 慎用隐式转换
	 danger_oops(100);
	 std::this_thread::sleep_for(std::chrono::seconds(2));

	return 0;
}

#endif
