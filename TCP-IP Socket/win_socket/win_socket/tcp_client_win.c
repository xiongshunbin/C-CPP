#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;		// 声明SOCKET变量以保存socket函数返回值。
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);	// 调用socket函数创建TCP套接字。
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	// while循环中调用recv函数读取数据，每次1个字节。
	while (readLen = recv(hSocket, &message[idx++], 1, 0))
	{
		if (readLen == SOCKET_ERROR)
			ErrorHandling("read() error!");

		// 每次读取1个字节，因此变量strLen每次加1，这与recv函数调用次数相同。
		strLen += readLen; if(message[idx - 1] == '\0')	break;
	}

	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}