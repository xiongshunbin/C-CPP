#pragma once

// ��ʼ�����ڼ������׽���
int initListenFd(unsigned short port);

// ����epoll
int epollRun(int lfd);

// �Ϳͻ��˽�������
int acceptClient(int lfd, int epfd);

// ����Http������Ϣ
int recvHttpRequest(int cfd, int epfd);

// ����������
int parseRequestLine(const char* line, int cfd);

// �����ļ�
int sendFile(const char* fileName, int cfd);