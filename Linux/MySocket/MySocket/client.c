#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "socket.h"

void* parseRecvMessage(void* arg)
{
	int fd = *(int*)arg;
	while (1)
	{
		char* buffer;
		enum Type t;
		int len = recvMessage(fd, &buffer, &t);
		if (buffer == NULL)
			continue;
		else
		{
			if (t == Heart)
			{
				printf("KeepAlive: %s......\n", buffer);
			}
			else
			{
				printf("Message: %s.\n", buffer);
			}
			free(*buffer);
		}
	}
	return NULL;
}

int main()
{
	const char* ip = "120.26.60.196";
	unsigned short port = 8888;

	int sockfd = initSocket();
	connectToHost(sockfd, ip, port);

	// 创建接收数据的子线程
	pthread_t pid;
	pthread_create(&pid, NULL, parseRecvMessage, &sockfd);

	while (1)
	{
		// 发送数据
		const char* data = "Hello, everyone!";
		sendMessage(sockfd, data, strlen(data), Message);
		sleep(2);
	}
	return 0;
}