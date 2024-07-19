#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(0);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0); // 创建UDP套接字，第二个参数传入SOCK_DGRAM
    if (serv_sock == -1)
        error_handling("UDP socket creation error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    while (1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, // 利用分配的地址接收数据，不限制数据传输对象。
                           (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        sendto(serv_sock, message, str_len, 0, // 向获取的数据传输端地址传输数据
               (struct sockaddr *)&clnt_adr, clnt_adr_sz);
    }
    close(serv_sock); // while()内部未加入break语句，因此是无限循环，该语句不会执行。
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}