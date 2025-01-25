#ifndef _WAVE_H_
#define _WAVE_H_

#include "enemy_type.h"

#include <vector>

struct Wave
{
	struct SpawnEvent
	{
		double interval = 0;				// ������һ��ˢ�ֵ�ʱ����
		int spawn_point = 1;				// ����λ��
		EnemyType enemy_type = EnemyType::Slim;
	};

	double rewards = 0;						// ����
	double interval = 0;					// ���μ��
	std::vector<SpawnEvent> spawn_event_list;
};

#endif // !_WAVE_H_
