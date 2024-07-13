#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 初始化套接字库
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		ErrorHandling(("WSAStartup() error!"));

	// 创建套接字
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket( error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// 给创建的套接字分配IP地址和端口号
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// 调用listen函数使创建的套接字成为服务器套接字
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	szClntAddr = sizeof(clntAddr);
	// 调用accept函数受理客户端连接请求
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	// 调用send函数向连接的客户端传输数据
	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	// 程序终止前注销初始化的套接字库
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}