#pragma once

// ��ʼ�����ڼ������׽���
int initListenFd(unsigned short port);

// ����epoll
int epollRun(int lfd);

// �Ϳͻ��˽�������
int acceptClient(int lfd, int epfd);