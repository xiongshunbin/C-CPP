#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "TcpServer.h"

int main(int argc, char* argv[])
{
#if 1
	if (argc != 3)
	{
		printf("Usage : %s <port> <path>\n", argv[0]);
		return -1;
	}

	unsigned short port = atoi(argv[1]);
	// 切换服务器的工作目录
	chdir(argv[2]);
#else
	unsigned short port = 10000;
	chdir("/home/wsl-ubuntu/Test/luffy/");
#endif

	// 启动服务器
	struct TcpServer* server = tcpServerInit(port, 4);
	tcpServerRun(server);

	return 0;
}