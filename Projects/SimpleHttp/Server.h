#pragma once

// ��ʼ�����ڼ������׽���
int initListenFd(unsigned short port);

// ����epoll
int epollRun(int listenFd);

// �Ϳͻ��˽�������
