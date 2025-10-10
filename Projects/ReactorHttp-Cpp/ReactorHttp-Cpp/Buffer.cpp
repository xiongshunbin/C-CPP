#include "Buffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/socket.h>

Buffer::Buffer(int size) : m_capacity(size)
{
	m_data = (char*)malloc(size * sizeof(char));
	bzero(m_data, sizeof(m_data));
}

Buffer::~Buffer()
{
	if (m_data != nullptr)
	{
		free(m_data);
	}
}

void Buffer::extendRoom(int size)
{
	// 1.内存够用 - 不需要扩容
	if (writeableSize() >= size)
	{
		return;
	}
	// 2.内存需要合并才够用 - 不需要扩容
	// 剩余的可写的内存 + 已读的内存 > size
	else if (writeableSize() + m_readPos >= size)
	{
		// 得到未读的内存大小
		int readable = readableSize();
		// 移动内存
		memcpy(m_data, m_data + m_readPos, readable * sizeof(char));
		// 更新位置
		m_readPos = 0;
		m_writePos = readable;
	}
	// 3.内存不够用 - 需要扩容
	else
	{
		char* temp = static_cast<char*>(realloc(m_data, (m_capacity + size) * sizeof(char)));
		if (temp == nullptr)
		{
			return;		// 失败了
		}
		memset(temp + m_capacity, 0, size * sizeof(char));
		// 更新数据
		m_data = temp;
		m_capacity += size;
	}
}

int Buffer::appendString(const char* data, int size)
{
	if (data == nullptr || size <= 0)
	{
		return -1;
	}
	// 内存扩容
	extendRoom(size);
	// 数据拷贝
	memcpy(m_data + m_writePos, data, size * sizeof(char));
	// 更新数据
	m_writePos += size;
	return 0;
}

int Buffer::appendString(const char* data)
{
	int size = strlen(data);
	int ret = appendString(data, size);
	return ret;
}

int Buffer::appendString(std::string msg)
{
	int ret = appendString(msg.data());
	return ret;
}

int Buffer::socketRead(int fd)
{
	// read/recv/readv
	struct iovec vec[2];
	// 初始化数组元素
	int writeable = writeableSize();
	vec[0].iov_base = m_data + m_writePos;
	vec[0].iov_len = writeable;
	char* tmpbuffer = (char*)malloc(sizeof(char) * 40960);		// 40KB
	vec[1].iov_base = tmpbuffer;
	vec[1].iov_len = 40960;
	int result = readv(fd, vec, 2);
	if (result == -1)
	{
		return -1;
	}
	else if (result <= writeable)
	{
		m_writePos += result;
	}
	else
	{
		m_writePos = m_capacity;
		appendString(tmpbuffer, result - writeable);
	}
	free(tmpbuffer);
	return result;
}

const char* Buffer::findCRLF()
{
	/**
	* 1.strstr --> 大字符串中匹配子字符串(遇到\0结束)
	*		char *strstr(const char *haystack, const char *needle);
	*
	* 2.memmem --> 大数据块中匹配子数据块(需要指定数据块的大小)
	*		void *memmem(const void *haystack, size_t haystacklen,
	*				const void *needle, size_t needlelen);
	*/
	char* ptr = static_cast<char*>(memmem(m_data + m_readPos, readableSize(), "\r\n", 2));

	return ptr;
}

int Buffer::sendData(int fd)
{
	// 判断有无数据
	int readable = readableSize();
	if (readable > 0)
	{
		// 可能会产生 Broken pip 错误导致程序崩溃
		int count = send(fd, m_data + m_readPos, readable, MSG_NOSIGNAL);
		if (count > 0)
		{
			m_readPos += count;
			usleep(1);
		}
		return count;
	}
	return 0;
}
