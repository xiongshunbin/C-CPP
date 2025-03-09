#include "ChannelMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ChannelMap* channelMapInit(int size)
{
	struct ChannelMap* map = (struct ChannelMap*)malloc(sizeof(struct ChannelMap));
	map->list = (struct Channel**)malloc(size * sizeof(struct Channel*));
	memset(map->list, 0, size * sizeof(struct Channel*));
	map->size = size;
	return map;
}

void channelMapClear(struct ChannelMap* map)
{
	if (map != NULL)
	{
		for (int i = 0; i < map->size; ++i)
		{
			if (map->list[i] != NULL)
				free(map->list[i]);
			map->list[i] = NULL;
		}
		free(map->list);
		map->list = NULL;
	}
	map->size = 0;
}

bool makeMapRoom(struct ChannelMap* map, int newSize, int unitSize)
{
	if (map->size < newSize)
	{
		int curSize = map->size;
		// ����ÿ������ԭ����һ��
		while (curSize < newSize)
			curSize *= 2;
		// ���� realloc
		struct Channel** temp = realloc(map->list, curSize * unitSize);
		if (temp == NULL)
			return false;
		map->list = temp;
		memset(map->list[map->size], 0, (curSize - map->size) * unitSize);
		map->size = curSize;
	}
	return true;
}
