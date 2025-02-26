#include "Server.h"

#include <arpa/inet.h>
#include <sys/epoll.h>

int initListenFd(unsigned short port)
{
	// 1. �������ڼ������׽���
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenFd == -1)
	{
		perror("socket");
		return -1;
	}

	// 2. ���ö˿ڸ���
	int opt = 1;
	int ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
		return -1;
	}

	// 3. ��IP��ַ�Ͷ˿ں�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	ret = bind(listenFd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	// 4. ���ü���
	ret = listen(listenFd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}

	// 5. ���س�ʼ���õ����ڼ������׽���
	return listenFd;
}

int epollRun(int listenFd)
{
	// 1. ����epoll��
	int epfd = epoll_create(1);
	if (epfd == -1)
	{
		perror("epoll_create");
		return -1;
	}

	// 2. ���listenFd��epoll��
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenFd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

	// 3. ����¼�
	struct epoll_event evs[1024];
	int maxevs = sizeof(evs) / sizeof(struct epoll_event);
	while (1)
	{
		int nfds = epoll_wait(epfd, evs, sizeof maxevs, -1);
		for (int i = 0; i < nfds; i++)
		{
			int fd = evs[i].data.fd;
			if (fd == listenFd)
			{
				// ����������


			}
			else
			{
				// ����ͨ�ţ���Ҫ�ǽ��ܶԶ˷��͵�����


			}

		}
	}


}
