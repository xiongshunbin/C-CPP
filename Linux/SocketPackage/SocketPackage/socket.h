#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <arpa/inet.h>
#include <unistd.h>

// ��������
int setListen(int lfd, unsigned short port);		// �� + ����
int acceptConn(int lfd, struct sockaddr_in* addr);


// �ͻ���
int connectToServer(int fd, const char* ip, unsigned short port);


// ͨ��
int createSocket();
int sendMsg(int fd, const char* msg, int len);
int recvMsg(int fd, char** buf);
int closeSocket(int fd);


#endif // _SOCKET_H_