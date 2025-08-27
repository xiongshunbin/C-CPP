#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int initSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		perror("socket");
	return fd;
}

void initSockaddr(struct sockaddr* addr, const char* ip, unsigned short port)
{
	struct sockaddr_in* addrin = (struct sockaddr_in*)addr;
	addrin->sin_family = AF_INET;
	addrin->sin_addr.s_addr = inet_addr(ip);
	addrin->sin_port = htons(port);
}

int setListen(int lfd, unsigned short port)
{
	// 设置端口复用
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// 绑定本地ip和port
	struct sockaddr addr;
	initSockaddr(&addr, "0.0.0.0", port);	// INADDR_ANY
	int ret = bind(lfd, &addr, sizeof(addr));
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	// 设置监听
	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}
	return 0;
}

int acceptConnect(int lfd, struct sockaddr* addr)
{
	int connfd;
	if (addr == NULL)
		connfd = accept(lfd, NULL, NULL);
	else
	{
		socklen_t len = sizeof(struct sockaddr);
		connfd = accept(lfd, addr, &len);
	}

	if (connfd == -1)
		perror("accept");
	return connfd;
}

int connectToHost(int fd, const char* ip, unsigned short port)
{
	struct sockaddr addr;
	initSockaddr(&addr, ip, port);
	int ret = connect(fd, &addr, sizeof(addr));
	if (ret == -1)
		perror("connect");
	return ret;
}

int readn(int fd, char* buffer, int size)
{
	int left = size;
	int readBytes = 0;
	char* ptr = buffer;
	while (left)
	{
		readBytes = read(fd, ptr, left);
		if (readBytes == -1)
		{
			if (errno == EINTR)
				readBytes = 0;
			else
			{
				perror("read");
				return -1;
			}
		}
		else if (readBytes == 0)
		{
			printf("对方主动断开了连接...\n");
			return -1;
		}
		ptr += readBytes;
		left -= readBytes;
	}
	return size;
}

int writen(int fd, const char* buffer, int length)
{
	int left = length;
	int writeBytes = 0;
	const char* ptr = buffer;

	while (left)
	{
		writeBytes = write(fd, ptr, left);
		if (writeBytes == -1)
		{
			if (errno == EINTR)
				writeBytes = 0;
			else
			{
				perror("write");
				return -1;
			}
		}
		ptr += writeBytes;
		left -= writeBytes;
	}
	return length;
}

int recvMessage(int fd, char** buffer, enum Type* t)
{
	unsigned int dataLen = 0;
	int ret = readn(fd, &dataLen, sizeof(unsigned int));
	if (ret == -1)
	{
		*buffer = NULL;
		return -1;
	}
	dataLen = ntohl(dataLen);
	char ch;
	readn(fd, &ch, sizeof(char));
	*t = ch == 'H' ? Heart : Message;
	char* tmpbuf = (char*)calloc(dataLen, sizeof(char));
	if (tmpbuf == NULL)
	{
		*buffer = NULL;
		return -1;
	}
	ret = readn(fd, tmpbuf, dataLen - 1);
	if (ret != dataLen - 1)
	{
		free(tmpbuf);
		*buffer = NULL;
		return -1;
	}
	*buffer = tmpbuf;
	return ret;
}

bool sendMessage(int fd, const char* buffer, int length, enum Type t)
{
	int dataLen = sizeof(unsigned int) + sizeof(char) + length;
	// 申请堆内存
	char* data = (char*)malloc(dataLen * sizeof(char));
	if (data == NULL)
	{
		return false;
	}
	unsigned int netLen = htonl(length + sizeof(char));
	memcpy(data, &netLen, sizeof(unsigned int));
	const char* ch = t == Heart ? "H" : "M";
	memcpy(data + sizeof(unsigned int), &ch, sizeof(char));
	memcpy(data + sizeof(unsigned int) + sizeof(char), buffer, length);
	int ret = writen(fd, data, dataLen);
	free(data);
	return ret == dataLen;
}
