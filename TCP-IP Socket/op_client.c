#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4 // 运算结果的字节数
#define OPSZ 4     // 待算数字的字节数
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE]; // 为收发数据准备的内存空间，需要数据积累到一定程度后再收发，因此通过数组创建。
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);    // 从程序用户的输入中得到待算数个数后，保存至数组opmsg。
    opmsg[0] = (char)opnd_cnt; // 强制转换成char类型，因为协议规定待算数个数应通过1字节整数型传递，因此不能超过1字节整数型能够表示的范围。

    // 从程序用户的输入中得到待算整数，保存到数组opmsg。4字节int型数据要保存到char数组，因而转换成int指针类型。
    for (int i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    fgetc(stdin); // 要输入字符，在此之前调用fgetc函数删掉缓冲区中的字符\n
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]); // 最后输入运算符信息，保存到opmsg数组。
    write(sock, opmsg, opnd_cnt * OPSZ + 2);  // 调用write函数一次性传输opmsg数组中的运算相关信息。可以调用1次write函数进行传输，也可以分成多次调用。
    read(sock, &result, RLT_SIZE);            // 保存服务器端传输的运算结果。待接收的数据长度为4字节，因此调用1次read函数即可接收。

    printf("Operation result: %d\n", result);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}