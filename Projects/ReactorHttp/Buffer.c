﻿#define _GNU_SOURCE
#include "Buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/socket.h>

struct Buffer* bufferInit(int size)
{
	struct Buffer* buffer = (struct Buffer*)malloc(sizeof(struct Buffer));
	if (buffer != NULL)
	{
		buffer->data = (char*)malloc(sizeof(char) * size);
		buffer->capacity = size;
		buffer->readPos = 0;
		buffer->writePos = 0;
		memset(buffer->data, 0, size);
	}

	return buffer;
}

void bufferDestroy(struct Buffer* buffer)
{
	if (buffer != NULL)
	{
		if (buffer->data != NULL)
		{
			free(buffer->data);
		}
	}
	free(buffer);
}

void bufferExtendRoom(struct Buffer* buffer, int size)
{
	// 1.内存够用 - 不需要扩容
	if (bufferWriteableSize(buffer) >= size)
		return;
	// 2.内存需要合并才够用 - 不需要扩容
	// 剩余的可写的内存 + 已读的内存 > size
	else if (bufferWriteableSize(buffer) + buffer->readPos >= size)
	{
		// 得到未读的内存大小
		int readable = bufferReadableSize(buffer);
		// 移动内存
		memcpy(buffer->data, buffer->data + buffer->readPos, readable);
		// 更新位置
		buffer->readPos = 0;
		buffer->writePos = readable;
	}
	// 3.内存不够用 - 需要扩容
	else
	{
		void* temp = realloc(buffer->data, buffer->capacity + size);
		if (temp == NULL)
		{
			return;		// 失败了
		}
		memset(temp + buffer->capacity, 0, size);
		// 更新数据
		buffer->data = temp;
		buffer->capacity += size;
	}
}

int bufferWriteableSize(struct Buffer* buffer)
{
	return buffer->capacity - buffer->writePos;
}

int bufferReadableSize(struct Buffer* buffer)
{
	return buffer->writePos - buffer->readPos;
}

int bufferAppendData(struct Buffer* buffer, const char* data, int size)
{
	if (buffer == NULL || data == NULL || size <= 0)
	{
		return -1;
	}
	// 内存扩容
	bufferExtendRoom(buffer, size);
	// 数据拷贝
	memcpy(buffer->data + buffer->writePos, data, size);
	// 更新数据
	buffer->writePos += size;
	return 0;
}

int bufferAppendString(struct Buffer* buffer, const char* data)
{
	int size = strlen(data);
	int ret = bufferAppendData(buffer, data, size);
	return ret;
}

int bufferSocketRead(struct Buffer* buffer, int fd)
{
	// read/recv/readv
	struct iovec vec[2];
	// 初始化数组元素
	int writeable = bufferWriteableSize(buffer);
	vec[0].iov_base = buffer->data + buffer->writePos;
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
		buffer->writePos += result;
	}
	else
	{
		buffer->writePos = buffer->capacity;
		bufferAppendData(buffer, tmpbuffer, result - writeable);
	}
	free(tmpbuffer);
	return result;
}

char* bufferFindCRLF(struct Buffer* buffer)
{
	/**
	 * 1.strstr --> 大字符串中匹配子字符串(遇到\0结束)
	 *		char *strstr(const char *haystack, const char *needle);
	 * 
	 * 2.memmem --> 大数据块中匹配子数据块(需要指定数据块的大小)
	 *		void *memmem(const void *haystack, size_t haystacklen,
	 *				const void *needle, size_t needlelen);
	 */
	char* ptr = (char*)memmem(buffer->data + buffer->readPos, bufferReadableSize(buffer), "\r\n", 2);

	return ptr;
}

int bufferSendData(struct Buffer* buffer, int fd)
{
	// 判断有无数据
	int readable = bufferReadableSize(buffer);
	if (readable > 0)
	{
		// 可能会产生 Broken pip 错误导致程序崩溃
		int count = send(fd, buffer->data + buffer->readPos, readable, MSG_NOSIGNAL);
		if (count > 0)
		{
			buffer->readPos += count;
			usleep(1);
		}
		return count;
	}
	return 0;
}

