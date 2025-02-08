#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <arpa/inet.h>
#include <unistd.h>

// 服务器端
int setListen(int lfd, unsigned short port);		// 绑定 + 监听
int acceptConn(int lfd, struct sockaddr_in* addr);


// 客户端
int connectToServer(int fd, const char* ip, unsigned short port);


// 通用
int createSocket();
int sendMsg(int fd, const char* msg, int len);
int recvMsg(int fd, char** buf);
int closeSocket(int fd);


#endif // _SOCKET_H_