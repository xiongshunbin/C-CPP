#undef UNICODE		// #undef����ȡ��֮ǰ����ĺꡣ������Ŀ������VC++������������2���꣬�����ں������õĺ���
#undef _UNICODE		// �У������ͽ�ת����unicode��ʽ��������������н��
#include <stdio.h>
#include <WinSock2.h>
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	char* strAddr = "203.211.218.102:9190";		// ��ת�����ַ�����ʽ�ĵ�ַ

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	size = sizeof(servAddr);
	WSAStringToAddress(
		strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);		// WSAStringToAddress���ַ���ת���ɽṹ��

	size = sizeof(strAddrBuf);
	WSAAddressToString(
		(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, (long*)&size);	// �ѽṹ��ת�����ַ���

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