#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "socket.h"

typedef struct SockInfo
{
	struct sockaddr_in addr;
	int fd;
}SockInfo;

SockInfo infos[512];


void* working(void* arg);

int main()
{
	// 1. 创建监听的套接字
	int fd = createSocket();
	if (fd == -1)
		return -1;

	// 2. 绑定本地IP和端口 + 设置监听
	int ret = setListen(fd, 10000);
	if (ret == -1)
		return -1;

	int max = sizeof(infos) / sizeof(infos[0]);
	for (int i = 0; i < max; i++)
	{
		bzero(&infos[i], sizeof(infos[i]));
		infos[i].fd = -1;
	}

	// 3. 阻塞并等待客户端的连接
	while (1)
	{
		SockInfo* pinfo = NULL;
		for (int i = 0; i < max; i++)
		{
			if (infos[i].fd == -1)
			{
				pinfo = &infos[i];
				break;
			}
		}
		pinfo->fd = acceptConn(fd, &pinfo->addr);
		if(pinfo->fd == -1)
			continue;
		// 创建子线程
		pthread_t tid;
		pthread_create(&tid, NULL, working, pinfo);
		pthread_detach(tid);
	}

	close(fd);

	return 0;
}

void* working(void* arg)
{
	SockInfo* pinfo = (SockInfo*)arg;
	// 连接建立成功，打印客户端的IP和端口信息
	char ip[32];
	printf("Client IP: %s, port: %d\n", inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(pinfo->addr.sin_port));

	while (1)
	{
		// 通信
		char* buf;
		int len = recvMsg(pinfo->fd, &buf);
		printf("Receive Data, %d: ......\n", len);
		if (len > 0)
		{
			printf("%s\n\n\n\n", buf);
			free(buf);
		}
		else
			break;

		sleep(1);
	}

	pinfo->fd = -1;
	close(pinfo->fd);

	return NULL;
}
