#include <stdio.h>
#include <unistd.h>
#include "common.h"

int readn(int fd, char* buf, int length)
{
    int nleft = length;
    int nread = 0;
    char* pbuf = buf;
    while(nleft > 0)
    {
        nread = read(fd, pbuf, nleft);
        if(nread == -1)
        {
            perror("read");
            return -1;
        }
        else if(nread == 0)
        {
            return length - nleft;
        }
        pbuf += nread;
        nleft -= nread;
    }
    return length;
}

int writen(int fd, const char* buf, int length)
{
    int nleft = length;
    int nwrite = 0;
    const char* pbuf = buf;
    while(nleft > 0)
    {
        nwrite = write(fd, pbuf, nleft);
        if(nwrite == -1)
        {
            perror("write");
            return -1;
        }
        else if(nwrite == 0)
        {
            continue;
        }
        pbuf += nwrite;
        nleft -= nwrite;
    }
    return length;
}