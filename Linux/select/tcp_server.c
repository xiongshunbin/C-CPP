#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>

pthread_mutex_t mutex;

typedef struct FDInfo
{
    int fd;
    int* maxfd;
    fd_set* rdset;
}FDInfo;

void* acceptConn(void* arg)
{
    // 接受客户端的连接
    printf("child thread ID: %ld\n", pthread_self());
    FDInfo* info = (FDInfo*)arg;
    int cfd = accept(info->fd, NULL, NULL);
    pthread_mutex_lock(&mutex);
    FD_SET(cfd, info->rdset);
    *info->maxfd = cfd > *info->maxfd ? cfd : *info->maxfd;
    pthread_mutex_unlock(&mutex);

    free(info);

    return NULL;
}

void* communication(void* arg)
{
    // 接收数据
    printf("child thread ID: %ld\n", pthread_self());
    FDInfo* info = (FDInfo*)arg;
    char buf[1024];
    int len = recv(info->fd, buf, sizeof(buf), 0);
    if(len == -1)
    {
        perror("recv() error");
        free(info);
        return NULL;
    }
    else if(len == 0)
    {
        printf("The client has disconnected......\n");
        pthread_mutex_lock(&mutex);
        FD_CLR(info->fd, info->rdset);
        pthread_mutex_unlock(&mutex);
        close(info->fd);
        free(info);
        return NULL;
    }
    printf("read buf = %s\n", buf);
    for(int i = 0; i < len; i ++)
        buf[i] = toupper(buf[i]);
    printf("after buf = %s\n", buf);

    int ret = send(info->fd, buf, strlen(buf) + 1, 0);
    if(ret == -1)
        perror("send() error");

    free(info);
    return NULL;
}

// server
int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);

    // 创建用于监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket() error");
        exit(1);
    }

    // 绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定端口
    int ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        perror("bind() error");
        exit(1);
    }

    // 监听
    ret = listen(lfd, 64);
    if(ret == -1)
    {
        perror("listen() error");
        exit(1);
    }

    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(lfd, &readset);

    int maxfd = lfd;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        fd_set tmp = readset;
        pthread_mutex_unlock(&mutex);
        ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        // 判断是否为监听的文件描述符
        if(FD_ISSET(lfd, &tmp))
        {
            // 创建子线程,用于接受客户端的连接
            pthread_t tid;
            FDInfo* info = (FDInfo*)malloc(sizeof(FDInfo));
            info->fd = lfd;
            info->maxfd = &maxfd;
            info->rdset = &readset;
            pthread_create(&tid, NULL, acceptConn, info);
            pthread_detach(tid);
        }
        for(int i = 0; i <= maxfd; i ++)
        {
            if(i != lfd && FD_ISSET(i, &tmp))
            {
                pthread_t tid;
                FDInfo* info = (FDInfo*)malloc(sizeof(FDInfo));
                info->fd = i;
                info->rdset = &readset;
                pthread_create(&tid, NULL, communication, info);
                pthread_detach(tid);
            }
        }
    }

    close(lfd);
    pthread_mutex_destroy(&mutex);

    return 0;
}