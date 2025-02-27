#include "Server.h"

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <fcntl.h>

int initListenFd(unsigned short port)
{
	// 1. �������ڼ������׽���
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket");
		return -1;
	}

	// 2. ���ö˿ڸ���
	int opt = 1;
	int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
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
	ret = bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	// 4. ���ü���
	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}

	// 5. ���س�ʼ���õ����ڼ������׽���
	return lfd;
}

int epollRun(int lfd)
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
	ev.data.fd = lfd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
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
			if (fd == lfd)
			{
				// ����������
				acceptClient(lfd, epfd);
			}
			else
			{
				// ����ͨ�ţ���Ҫ�ǽ��ܶԶ˷��͵�����


			}

		}
	}


}

int acceptClient(int lfd, int epfd)
{
	// 1. ��������
	int cfd = accept(lfd, NULL, NULL);
	if (cfd == -1)
	{
		perror("accept");
		return -1;
	}
	// 2. ���÷�����
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcnt(cfd, F_SETFL, flag);

	// 3. cfd��ӵ�epoll��
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = cfd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

	return 0;
}
