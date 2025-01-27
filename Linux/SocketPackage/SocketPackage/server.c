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
	// 1. �����������׽���
	int fd = createSocket();
	if (fd == -1)
		return -1;

	// 2. �󶨱���IP�Ͷ˿� + ���ü���
	int ret = setListen(fd, 10000);
	if (ret == -1)
		return -1;

	int max = sizeof(infos) / sizeof(infos[0]);
	for (int i = 0; i < max; i++)
	{
		bzero(&infos[i], sizeof(infos[i]));
		infos[i].fd = -1;
	}

	// 3. �������ȴ��ͻ��˵�����
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
		// �������߳�
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
	// ���ӽ����ɹ�����ӡ�ͻ��˵�IP�Ͷ˿���Ϣ
	char ip[32];
	printf("Client IP: %s, port: %d\n", inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(pinfo->addr.sin_port));

	while (1)
	{
		// ͨ��
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
