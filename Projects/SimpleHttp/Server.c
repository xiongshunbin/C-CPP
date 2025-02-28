#include "Server.h"
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

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
				recvHttpRequest(fd, epfd);

			}

		}
	}

	return 0;
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

int recvHttpRequest(int cfd, int epfd)
{
	int len = 0, total = 0;
	char tmp[1024] = { 0 };
	char buf[4096] = { 0 };
	while (len = recv(cfd, tmp, sizeof tmp, 0) > 0)
	{
		if (total + len < sizeof buf)
			memcpy(buf + total, tmp, len);
		total += len;
	}
	// �ж������Ƿ񱻽������
	if (len == -1 && errno == EAGAIN)
	{
		// ����������

	}
	else if (len = 0)
	{
		// �ͻ��˶Ͽ�������
		int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
		if (ret == -1)
		{
			perror("epoll_ctl");
			return -1;
		}
		close(cfd);
	}
	else
	{
		perror("recv");
		return -1;
	}
	return 0;
}

int parseRequestLine(const char* line, int cfd)
{
	// ���������� get /xxx/1.jpg http/1.1
	char method[12];
	char path[1024];
	sscanf(line, "%[^ ] %[^ ]", method, path);
	if (strcasecmp(method, "get") != 0)
		return -1;
	// ����ͻ�������ľ�̬��Դ(Ŀ¼ or �ļ�)
	const char* file = NULL;
	if (strcmpy(path, "/") == 0)
		file = "./";
	else
		file = path + 1;
	// ��ȡ�ļ�����
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// �ļ������� -- �ظ�404

	}
	// �ж��ļ�����
	if (S_ISDIR(st.st_mode))
	{
		// �ѱ���Ŀ¼�е����ݷ��͸��ͻ���

	}
	else
	{
		// ���ļ������ݷ��͸��ͻ���

	}

	return 0;
}

int sendFile(const char* fileName, int cfd)
{
	// 1. ���ļ�
	int fd = open(fileName, O_RDONLY);
	assert(fd > 0);
	// 2. ��ȡ�ļ��е����ݲ����͵��ͻ���
	char buf[1024];
	while (1)
	{
		int len = read(fd, buf, sizeof buf);
		if (len > 0)
		{
			send(cfd, buf, len, 0);

		}
	}
	

	return 0;
}
