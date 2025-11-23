#include <iostream>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
using namespace std;

// 单例模式
// 定义：在一个项目中，全局范围内，某个类的实例有且仅有一个，通过这个唯一实例向其他模块提供数据的全局访问，这种模式就叫单例模式。

/*
* 对于一个空类，系统默认提供的成员函数：
*	默认提供的函数
*	1. 默认构造函数（不带任何参数）
*	2. 拷贝构造函数
*	3. 移动构造函数
*	4. 析构函数
*	5. 拷贝复制操作符重载
*	6. 移动复制操作符重载
*/

// 定义一个单例模式的任务队列
#if 0
// 饿汉模式 -> 定义类的时候创建单例对象
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		return m_taskQ;
	}

	void print()
	{
		cout << "我是单例对象的一个成员函数..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;

	// 只能通过类名访问静态属性或方法
	static TaskQueue* m_taskQ;
};

// 静态变量不能够在类的内部进行初始化
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

#endif

#if 0
// 懒汉模式 -> 使用的时候再去创建单例对象的实例
// 使用双重检查锁定和原子操作解决多线程场景下懒汉模式的线程安全问题
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		// 原子变量
		TaskQueue* task = m_taskQ.load();
		// 双重检查锁定：只有第一次时线程是顺序访问的，后续线程就可并行访问单例对象
		if (task == nullptr)
		{
			// 第一次串行顺序访问
			m_mutex.lock();
			task = m_taskQ.load();
			if (task == nullptr)
			{
				task = new TaskQueue;
				m_taskQ.store(task);
			}
			m_mutex.unlock();
		}
		return task;
	}

	void print()
	{
		cout << "我是单例对象的一个成员函数..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;

	// 只能通过类名访问静态属性或方法
	// static TaskQueue* m_taskQ;
	static mutex m_mutex;
	static atomic<TaskQueue*> m_taskQ;
};

// 静态变量不能够在类的内部进行初始化
// TaskQueue* TaskQueue::m_taskQ = nullptr;
atomic<TaskQueue*> TaskQueue::m_taskQ;
mutex TaskQueue::m_mutex;

#endif

#if 0
// 懒汉模式 -> 使用的时候再去创建单例对象的实例
// 使用静态的局部对象解决线程安全问题 -> 编译器必须支持C++11
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		// 如果指令逻辑进入一个未被初始化的声明变量，所有并发执行应当等待该变量完成初始化。
		static TaskQueue task;
		return &task;
	}

	void print()
	{
		cout << "我是单例对象的一个成员函数..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;
};

#endif

/*
* 饿汉模式和懒汉模式的比较：
* 1. 懒汉模式更节约内存资源
* 2. 在多线程的场景下，饿汉模式在多个线程访问单例对象的时候没有线程安全问题，懒汉模式存在线程安全问题
* 3. 解决多线程场景下懒汉模式存在的线程安全问题，可采用互斥锁，多个线程顺序访问单例对象，操作更复杂且效率更低
*/

#if 1
// 替巴基写一个任务队列
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		return m_taskQ;
	}

	void print()
	{
		cout << "我是单例对象的一个成员函数..." << endl;
	}

	// 判断任务队列是否为空
	bool isEmpty()
	{
		lock_guard<mutex> locker(m_mutex);
		bool flag = m_data.empty();
		return flag;
	}
	// 添加任务
	void addTask(int node)
	{
		lock_guard<mutex> locker(m_mutex);
		m_data.push(node);
	}
	// 删除任务
	bool popTask()
	{
		lock_guard<mutex> locker(m_mutex);
		if (m_data.empty())
			return false;
		m_data.pop();
		return true;
	}
	// 取出一个任务(不删除任务)
	int takeTask()
	{
		lock_guard<mutex> locker(m_mutex);
		if (m_data.empty())
			return -1;
		int data = m_data.front();
		return data;
	}

private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;
	// 只能通过类名访问静态属性或方法
	static TaskQueue* m_taskQ;
	// 定义任务队列
	queue<int> m_data;
	mutex m_mutex;
};

TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

#endif

#if 0

int main()		// 主线程
{
	TaskQueue* taskQ = TaskQueue::getInstance();
	
	// 生产者
	thread t1([=]()
		{
			for (int i = 0; i < 10; i++)
			{
				taskQ->addTask(i + 100);
				cout << "+++ push data: " << i + 100 << ", threadID: " <<
					this_thread::get_id() << endl;
				this_thread::sleep_for(chrono::milliseconds(500));
			}
		});
	

	// 消费者
	thread t2([=]()
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			while(!taskQ->isEmpty())
			{
				int num = taskQ->takeTask();
				cout << "+++ take data: " << num << ", threadID: " <<
					this_thread::get_id() << endl;
				taskQ->popTask();
				this_thread::sleep_for(chrono::milliseconds(1000));
			}
		});

	t1.join();		// join()作用： 阻塞主线程
	t2.join();		// 当t1和t2线程任务完成之后，主线程才会解除阻塞

	return 0;
}

#endif