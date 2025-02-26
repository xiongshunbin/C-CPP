#pragma once

#include <thread>
#include <vector>
#include <map>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <memory>
#include <condition_variable>


/*
* ���ɣ�
*	1.�������߳�	->	���̣߳�1��
*		- ���ƹ����̵߳����������ӻ����
*	2.�������߳�	->	���̣߳�n��
*		- �����������ȡ���񣬲�����
*		- ���������Ϊ�գ�������������
*		- �߳�ͬ������������
*		- ��С����󡢵�ǰ�������߳�����
*	3.�������	->	STL	-> queue
*		- ������
*		- ��������
*	4. �̳߳ؿ���	-> bool
*/

class ThreadPool
{
public:
	ThreadPool(int min = 2, int max = std::thread::hardware_concurrency());
	~ThreadPool();

	// �������	->	�������
	void addTask(std::function<void(void)> task);

	template <typename F, typename... Args>
	auto addTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
	{
		// 1. ����package_taskʵ��
		using returnType = typename std::result_of<F(Args...)>::type;
		auto mytask = std::make_shared<std::packaged_task<returnType()>> (
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);
		// 2. �õ�future
		std::future<returnType> res = mytask->get_future();
		// 3. ��������ӵ��������
		m_mutexQueue.lock();
		m_taskQ.emplace([mytask]() {
			(*mytask)();
			});
		m_mutexQueue.unlock();
		m_condition.notify_one();

		return res;
	}

private:
	void manager(void);
	void worker(void);

private:
	std::thread* m_manager;								// �������߳�
	std::map<std::thread::id, std::thread> m_workers;	// �������߳�
	std::vector<std::thread::id> m_ids;					// �˳��̺߳������߳�ID
	std::atomic<int> m_minThread;						// ��С�߳���
	std::atomic<int> m_maxThread;						// ����߳���
	std::atomic<int> m_curThread;						// ��ǰ�߳���
	std::atomic<int> m_idleThread;						// �����߳���
	std::atomic<int> m_exitThread;						// �˳��߳���
	std::atomic<bool> m_stop;							// �̳߳ؿ���
	std::queue<std::function<void(void)>> m_taskQ;		// �������
	std::mutex m_mutexQueue;							// �����������ڶ�������еĻ������
	std::mutex m_mutexIds;								// �����������ڶԣ�m_ids�Ļ������
	std::condition_variable m_condition;				// �������������������Ϊ��ʱ�����������߽���
};

