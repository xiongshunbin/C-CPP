#pragma once
#include <functional>
#include <cstdint>

// 定义文件描述符的读写事件
enum class FDEvent : uint8_t
{
	TimeOut = 0x01,
	ReadEvent = 0x02,
	WriteEvent = 0x04,
	ReadWriteEvent = ReadEvent | WriteEvent
};

/**
 * 可调用对象包装器特点: 
 *		1.包装的是函数指针(函数的地址 -- 函数名)或可调用对象(可以像函数一样使用的对象)
 *		2.最终得到地址, 但未被调用
 */

class Channel
{
public:
	using handleFunc = std::function<int(void* arg)>;
	Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
	~Channel();

	// 修改fd的写事件(检测 or 不检测)
	void writeEventEnable(bool flag);

	// 判断是否检测文件描述符的写事件
	bool isWriteEventEnable();

	// 取出私有数据成员的值
	inline int getSocketFd() const { return m_fd; }
	inline int getEvent() const { return m_events; }
	inline const void* getArg() const { return m_arg; }

private:
	int m_fd;						// 文件描述符
	int m_events;					// 事件
	void* m_arg = nullptr;			// 回调函数的参数

public:
	handleFunc readCallback = nullptr;		// 读回调
	handleFunc writeCallback = nullptr;		// 写回调
	handleFunc destroyCallback = nullptr;	// 销毁回调
};