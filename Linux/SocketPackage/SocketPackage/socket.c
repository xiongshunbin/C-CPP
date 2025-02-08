#include "socket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// �󶨱���IP�Ͷ˿ڣ����ü���
int setListen(int lfd, unsigned short port)
{
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret == -1)
	{
		perror("bind() error");
		return -1;
	}
	printf("Bind successful.\n");

	ret = listen(lfd, 10);
	if (ret == -1)
	{
		perror("listen() error");
		return -1;
	}
	printf("Listen successful.\n");
	return ret;
}

// �������ȴ��ͻ�������
int acceptConn(int lfd, struct sockaddr_in* addr)
{
	int cfd = -1;
	if (addr == NULL)
		cfd = accept(lfd, NULL, NULL);
	else
	{
		int addrLen = sizeof(struct sockaddr_in);
		cfd = accept(lfd, (struct sockaddr*)addr, &addrLen);
	}
	if (cfd == -1)
	{
		perror("accept() error");
		return -1;
	}
	printf("Successfully established a connection with the client...\n");
	return cfd;
}

// ���ӷ�����
int connectToServer(int fd, const char* ip, unsigned short port)
{
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &saddr.sin_addr.s_addr);
	int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret == -1)
	{
		perror("connect() error");
		return -1;
	}
	printf("Successfully connected to the server...\n");
	return ret;
}

// �����׽���
int createSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket() error");
		return -1;
	}
	printf("Socket creation successful, fd = %d\n", fd);
	return fd;
}

// ����ָ�����ȵ��ַ���
int writeN(int fd, const char* msg, int size)
{
	const char* buf = msg;
	int remaining = size;
	while (remaining > 0)
	{
		int len = send(fd, buf, remaining, 0);
		if (len == -1)
		{
			perror("send() error");
			return -1;
		}
		else if (len == 0)
			continue;
		buf += len;
		remaining -= len;
	}
	return size;
}

// ��������
int sendMsg(int fd, const char* msg, int len)
{
	if (fd < 0 || msg == NULL || len <= 0)
		return -1;

	char* data = (char*)malloc(sizeof(int) + len);
	if (data == NULL)
	{
		perror("malloc() error");
		return -1;
	}

	int bigLen = htonl(len);
	memcpy(data, &bigLen, sizeof(int));
	memcpy(data + sizeof(int), msg, len);

	int ret = writeN(fd, data, len + sizeof(int));
	if (ret == -1)
		close(fd);
	return ret;
}

// ����ָ�����ȵ��ַ���
int readN(int fd, char* buf, int size)
{
	char* ptr = buf;
	int remaining = size;
	while (remaining > 0)
	{
		int len = recv(fd, ptr, remaining, 0);
		if (len == -1)
			return -1;
		else if (len == 0)
			return size - remaining;
		ptr += len;
		remaining -= len;
	}
	return size;
}

// ��������
int recvMsg(int fd, char** buf)
{
	int len = 0;
	readN(fd, (char*)&len, sizeof(int));
	len = ntohl(len);
	printf("The length of the data block to be received is %d.\n", len);

	char* data = (char*)malloc(len + 1);
	int length = readN(fd, data, len);
	if (length != len)
	{
		printf("Failed to receive data.\n");
		close(fd);
		free(data);
		return -1;
	}
	data[len] = '\0';
	*buf = data;
	return length;
}

// �ر��׽���
int closeSocket(int fd)
{
	int ret = close(fd);
	if (ret == -1)
	{
		perror("close() error");
		return -1;
	}
	return ret;
}
