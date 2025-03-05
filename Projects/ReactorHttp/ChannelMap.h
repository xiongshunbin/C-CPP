#pragma once

#include "Channel.h"

struct ChannelMap
{
	int size;		// ��¼ָ��ָ��������Ԫ���ܸ���
	// struct Channel* list[];
	struct Channel** list;
};

// ��ʼ��Channel
struct ChannelMap* channelMapInit(int size);

// ���ChannelMap
void channelMapClear(struct ChannelMap* map);

// ���·����ڴ�ռ�
bool makeMapRoom(struct ChannelMap* map, int newSize, int unitSize);
