#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Server.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage : %s <port> <path>\n", argv[0]);
		return -1;
	}

	unsigned short port = atoi(argv[1]);
	// �л��������Ĺ���Ŀ¼
	chdir(argv[2]);

	// ��ʼ�����ڼ������׽���
	int lfd = initListenFd(port);

	// ��������������
	epollRun(lfd);

	return 0;
}