#pragma once

struct Buffer
{
	char* data;			// ָ���ڴ��ָ��
	int capacity;		// �ڴ��С
	int readPos;		// ��λ��
	int writePos;		// дλ��
};

// ��ʼ��
struct Buffer* bufferInit(int size);

// �����ڴ�
void bufferDestroy(struct Buffer* buffer);

// �ڴ�����
void bufferExtendRoom(struct Buffer* buffer, int size);

// �õ�ʣ��Ŀ�д���ڴ�����
int bufferWriteableSize(struct Buffer* buffer);

// �õ�ʣ��Ŀɶ�(δ��)���ڴ�����
int bufferReadableSize(struct Buffer* buffer);

// д�ڴ� 1.ֱ��д 2.�����׽�������
int bufferAppendData(struct Buffer* buffer, const char* data, int size);
int bufferAppendString(struct Buffer* buffer, const char* data);

int bufferSocketRead(struct Buffer* buffer, int fd);