#pragma once

// 定义函数指针
using handleFunc = int(*)(void* arg);

// 定义文件描述符的读写事件
enum class FDEvent
{
	TimeOut = 0x01,
	ReadEvent = 0x02,
	WriteEvent = 0x04
};

class Channel
{
public:
	Channel(int fd, int events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
	~Channel();

	// 修改fd的写事件(检测 or 不检测)
	void writeEventEnable(bool flag);

	// 判断是否检测文件描述符的写事件
	bool isWriteEventEnable();

	// 取出私有数据成员的值
	inline int getSocketFd() { return m_fd; }
	inline int getEvent() { return m_events; }
	inline void* getArg() { return m_arg; }

private:
	int m_fd;						// 文件描述符
	int m_events;					// 事件
	void* m_arg;					// 回调函数的参数

public:
	handleFunc readCallback;		// 读回调
	handleFunc writeCallback;		// 写回调
	handleFunc destroyCallback;		// 销毁回调
};