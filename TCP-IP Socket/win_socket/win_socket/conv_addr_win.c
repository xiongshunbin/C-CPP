#undef UNICODE		// #undef用于取消之前定义的宏。根据项目环境，VC++会自主声明这2个宏，这样在后续调用的函数
#undef _UNICODE		// 中，参数就将转换成unicode形式，给出错误的运行结果
#include <stdio.h>
#include <WinSock2.h>
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	char* strAddr = "203.211.218.102:9190";		// 待转换的字符串格式的地址

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	size = sizeof(servAddr);
	WSAStringToAddress(
		strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);		// WSAStringToAddress把字符串转换成结构体

	size = sizeof(strAddrBuf);
	WSAAddressToString(
		(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, (long*)&size);	// 把结构体转换成字符串

	printf("Second conv result: %s \n", strAddrBuf);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}