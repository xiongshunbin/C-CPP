// 单例模式

#include <iostream>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
using namespace std;

// 饿汉模式
#if 0
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
		static TaskQueue* m_taskQ;
};
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;
#endif

// 懒汉模式
// 使用双重检测锁定和原子操作解决多线程场景下懒汉模式存在的线程安全问题
#if 0
class TaskQueue
{
	public:
		TaskQueue(const TaskQueue& t) = delete;
		TaskQueue& operator=(const TaskQueue& t) = delete;
		static TaskQueue* getInstance()
		{
			TaskQueue* task = m_taskQ.load();
			if(task == nullptr)
			{
				m_mutex.lock();
				task = m_taskQ.load();
				if(task == nullptr)
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
		static atomic<TaskQueue*> m_taskQ;
		static mutex m_mutex;
};
mutex TaskQueue::m_mutex;
atomic<TaskQueue*> TaskQueue::m_taskQ;
#endif

#if 0
// 使用静态局部对象解决多线程多线程场景下懒汉模式的线程安全问题
class TaskQueue
{
	public:
		TaskQueue(const TaskQueue& t) = delete;
		TaskQueue& operator=(const TaskQueue& t) = delete;
		static TaskQueue* getInstance()
		{
			static TaskQueue task;;
			return &task;
		}
		void print()
		{
			cout << "我是单例对象的一个成员函数..." << endl;
		}
	private:
		TaskQueue() = default;
};

#endif

// 替巴基写一个任务队列
#if 1
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

		bool isEmpty()
		{
			lock_guard<mutex> locker(m_mutex); 
			bool flag = m_data.empty();
			return flag;
		}

		void addTask(int node)
		{
			lock_guard<mutex> locker(m_mutex); 
			m_data.push(node);
		}

		bool popTask()
		{
			lock_guard<mutex> locker(m_mutex); 
			if(m_data.empty())
				return false;
			m_data.pop();
			return true;
		}

		int takeTask()
		{
			lock_guard<mutex> locker(m_mutex); 
			if(m_data.empty())
				return -1;
			int data = m_data.front();
			return data;
		}
	private:
		TaskQueue() = default;
		static TaskQueue* m_taskQ;
		mutex m_mutex;
		queue<int> m_data;
};
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;
#endif

int main()
{
	TaskQueue* task = TaskQueue::getInstance();
	thread t1([=]()
	{
		for(int i = 0; i < 10; i ++)
		{
			task->addTask(i + 100);
			cout << "+++ push data: " << i + 100 << ", threadID: " << this_thread::get_id() << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
	});

	thread t2([=]()
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		while(!task->isEmpty())
		{
			int num = task->takeTask();
			cout << "+++ take data: " << num << ", threadID: " << this_thread::get_id() << endl;
			task->popTask();
			this_thread::sleep_for(chrono::milliseconds(1000));
		}
	});

	t1.join();
	t2.join();

	return 0;
}
