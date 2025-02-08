#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>s

/*
* ���ɣ�
*	1.�������߳�	->	���̣߳�1��
*		- ���ƹ����̵߳����������ӻ����
*	2.�������߳�	->	���̣߳�n��
*		- �����������ȡ���񣬲�����
*		- ���������Ϊ�գ�������������������������
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

private:
	std::thread* m_manager;
	std::vector<std::thread> m_workers;
	std::atomic<int> m_minThread;
	std::atomic<int> m_maxThread;
	std::atomic<int> m_curThread;
	std::atomic<int> m_idleThread;
	std::atomic<bool> m_stop;
	std::queue<std::function<void(void)>> m_task;
	std::mutex m_mutexQueue;
	std::condition_variable m_condition;
};

