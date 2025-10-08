﻿#pragma once

class Buffer
{
public:
	explicit Buffer(int size);
	~Buffer();

	// 内存扩容
	void extendRoom(int size);

	// 写内存 1.直接写 2.接收套接字数据
	int appendString(const char* data, int size);
	int appendString(const char* data);
	int socketRead(int fd);

	// 根据\r\n取出一行, 找到其在数据块中的位置, 返回该位置
	char* findCRLF();

	// 发送数据
	int sendData(int fd);

	// 得到剩余的可写的内存容量
	inline int writeableSize() { return m_capacity - m_writePos; }

	// 得到剩余的可读(未读)的内存容量
	inline int readableSize() { return m_writePos - m_readPos; }

private:
	char* m_data = nullptr;		// 指向内存的指针
	int m_capacity = 0;			// 内存大小
	int m_readPos = 0;			// 标记读的相对位置
	int m_writePos = 0;			// 标记写的相对位置
};
