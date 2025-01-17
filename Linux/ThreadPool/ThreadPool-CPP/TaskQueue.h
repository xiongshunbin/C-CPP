#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_

#include <queue>
#include <pthread.h>

using callback = void (*)(void*);

template <typename T>
struct Task
{
	Task()
	{
		function = nullptr;
		arg = nullptr;
	}

	Task(callback f, void* a)
	{
		function = f;
		arg = (T*)a;
	}

	callback function;
	T* arg;
};

template <typename T>
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();

	// �������
	void addTask(const Task<T>& task);
	void addTask(callback f, void* arg);
	// ȡ������
	Task<T> takeTask();
	// ��ȡ�������
	inline size_t taskNumber()
	{
		return m_taskQ.size();
	}

private:
	std::queue<Task<T>> m_taskQ;
	pthread_mutex_t m_mutex;
};

#endif // _TASKQUEUE_H_

