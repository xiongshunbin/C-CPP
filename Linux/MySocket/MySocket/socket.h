#pragma once

#include <stdbool.h>

// ���ݰ�����
enum Type{Heart, Message};

/**
 * ���ݰ�����: 'H':������, 'M': ���ݰ�
 * ���ݰ���ʽ: ���ݳ��� | ���ݰ����� | ���ݿ�
 *				int		char		char*
 *				4�ֽ�	1�ֽ�		N�ֽ�
 */

// ��ʼ��һ���׽���
int initSocket();

// ��ʼ��sockaddr�ṹ��
void initSockaddr(struct sockaddr* addr, const char* ip, unsigned short port);

// ���ü���
int setListen(int lfd, unsigned short port);

// ���շ���������
int acceptConnect(int lfd, struct sockaddr* addr);

// ���ӷ�����
int connectToHost(int fd, const char* ip, unsigned short port);

// ����ָ�����ֽ���
int readn(int fd, char* buffer, int size);

// д��ָ�����ֽ���
int writen(int fd, const char* buffer, int length);

// ��������
bool sendMessage(int fd, const char* buffer, int length, enum Type t);

// ��������
int recvMessage(int fd, char** buffer, enum Type* t);
