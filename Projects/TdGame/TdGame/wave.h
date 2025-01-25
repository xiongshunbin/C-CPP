#ifndef _WAVE_H_
#define _WAVE_H_

#include "enemy_type.h"

#include <vector>

struct Wave
{
	struct SpawnEvent
	{
		double interval = 0;				// 距离上一次刷怪的时间间隔
		int spawn_point = 1;				// 生成位点
		EnemyType enemy_type = EnemyType::Slim;
	};

	double rewards = 0;						// 波次
	double interval = 0;					// 波次间隔
	std::vector<SpawnEvent> spawn_event_list;
};

#endif // !_WAVE_H_
