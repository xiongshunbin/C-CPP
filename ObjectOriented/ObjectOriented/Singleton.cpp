#include <iostream>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
using namespace std;

// ����ģʽ
// ���壺��һ����Ŀ�У�ȫ�ַ�Χ�ڣ�ĳ�����ʵ�����ҽ���һ����ͨ�����Ψһʵ��������ģ���ṩ���ݵ�ȫ�ַ��ʣ�����ģʽ�ͽе���ģʽ��

/*
* ����һ�����࣬ϵͳĬ���ṩ�ĳ�Ա������
*	Ĭ���ṩ�ĺ���
*	1. Ĭ�Ϲ��캯���������κβ�����
*	2. �������캯��
*	3. �ƶ����캯��
*	4. ��������
*	5. �������Ʋ���������
*	6. �ƶ����Ʋ���������
*/

// ����һ������ģʽ���������
#if 0
// ����ģʽ -> �������ʱ�򴴽���������
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
		cout << "���ǵ��������һ����Ա����..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;

	// ֻ��ͨ���������ʾ�̬���Ի򷽷�
	static TaskQueue* m_taskQ;
};

// ��̬�������ܹ�������ڲ����г�ʼ��
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

#endif

#if 0
// ����ģʽ -> ʹ�õ�ʱ����ȥ�������������ʵ��
// ʹ��˫�ؼ��������ԭ�Ӳ���������̳߳���������ģʽ���̰߳�ȫ����
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		// ԭ�ӱ���
		TaskQueue* task = m_taskQ.load();
		// ˫�ؼ��������ֻ�е�һ��ʱ�߳���˳����ʵģ������߳̾Ϳɲ��з��ʵ�������
		if (task == nullptr)
		{
			// ��һ�δ���˳�����
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
		cout << "���ǵ��������һ����Ա����..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;

	// ֻ��ͨ���������ʾ�̬���Ի򷽷�
	// static TaskQueue* m_taskQ;
	static mutex m_mutex;
	static atomic<TaskQueue*> m_taskQ;
};

// ��̬�������ܹ�������ڲ����г�ʼ��
// TaskQueue* TaskQueue::m_taskQ = nullptr;
atomic<TaskQueue*> TaskQueue::m_taskQ;
mutex TaskQueue::m_mutex;

#endif

#if 0
// ����ģʽ -> ʹ�õ�ʱ����ȥ�������������ʵ��
// ʹ�þ�̬�ľֲ��������̰߳�ȫ���� -> ����������֧��C++11
class TaskQueue
{
public:
	TaskQueue(const TaskQueue& t) = delete;
	TaskQueue& operator=(const TaskQueue& t) = delete;
	static TaskQueue* getInstance()
	{
		// ���ָ���߼�����һ��δ����ʼ�����������������в���ִ��Ӧ���ȴ��ñ�����ɳ�ʼ����
		static TaskQueue task;
		return &task;
	}

	void print()
	{
		cout << "���ǵ��������һ����Ա����..." << endl;
	}
private:
	TaskQueue() = default;
	// TaskQueue(const TaskQueue& t) = default;
	// TaskQueue& operator=(const TaskQueue& t) = default;
};

#endif

/*
* ����ģʽ������ģʽ�ıȽϣ�
* 1. ����ģʽ����Լ�ڴ���Դ
* 2. �ڶ��̵߳ĳ����£�����ģʽ�ڶ���̷߳��ʵ��������ʱ��û���̰߳�ȫ���⣬����ģʽ�����̰߳�ȫ����
* 3. ������̳߳���������ģʽ���ڵ��̰߳�ȫ���⣬�ɲ��û�����������߳�˳����ʵ������󣬲�����������Ч�ʸ���
*/

#if 1
// ��ͻ�дһ���������
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
		cout << "���ǵ��������һ����Ա����..." << endl;
	}

	// �ж���������Ƿ�Ϊ��
	bool isEmpty()
	{
		lock_guard<mutex> locker(m_mutex);
		bool flag = m_data.empty();
		return flag;
	}
	// �������
	void addTask(int node)
	{
		lock_guard<mutex> locker(m_mutex);
		m_data.push(node);
	}
	// ɾ������
	bool popTask()
	{
		lock_guard<mutex> locker(m_mutex);
		if (m_data.empty())
			return false;
		m_data.pop();
		return true;
	}
	// ȡ��һ������(��ɾ������)
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
	// ֻ��ͨ���������ʾ�̬���Ի򷽷�
	static TaskQueue* m_taskQ;
	// �����������
	queue<int> m_data;
	mutex m_mutex;
};

TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

#endif

# if 0

int main()		// ���߳�
{
	TaskQueue* taskQ = TaskQueue::getInstance();
	
	// ������
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
	

	// ������
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

	t1.join();		// join()���ã� �������߳�
	t2.join();		// ��t1��t2�߳��������֮�����̲߳Ż�������

	return 0;
}

#endif