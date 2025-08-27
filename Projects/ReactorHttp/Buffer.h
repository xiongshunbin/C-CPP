#pragma once

struct Buffer
{
	char* data;			// 指向内存的指针
	int capacity;		// 内存大小
	int readPos;		// 读位置
	int writePos;		// 写位置
};

// 初始化
struct Buffer* bufferInit(int size);

// 销毁内存
void bufferDestroy(struct Buffer* buffer);

// 内存扩容
void bufferExtendRoom(struct Buffer* buffer, int size);

// 得到剩余的可写的内存容量
int bufferWriteableSize(struct Buffer* buffer);

// 得到剩余的可读(未读)的内存容量
int bufferReadableSize(struct Buffer* buffer);

// 写内存 1.直接写 2.接收套接字数据
int bufferAppendData(struct Buffer* buffer, const char* data, int size);
int bufferAppendString(struct Buffer* buffer, const char* data);

int bufferSocketRead(struct Buffer* buffer, int fd);