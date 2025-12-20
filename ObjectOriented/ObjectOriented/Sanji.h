#pragma once

#include "State.h"

class Sanji
{
public:
	Sanji()
	{
		m_state = new ForenoonState;
	}

	~Sanji()
	{
		if (m_state != nullptr)
		{
			delete m_state;
		}
	}

	// 工作函数
	void working()
	{
		m_state->working(this);
	}

	// 切换状态
	void setState(AbstractState* state)
	{
		if (m_state != nullptr)
		{
			delete m_state;
			m_state = nullptr;
		}
		m_state = state;
	}

	// 获取时间
	int getClock()
	{
		return m_clock;
	}

	// 设置时间
	void setClock(int clock)
	{
		m_clock = clock;
	}

private:
	int m_clock = 0;					// 当前时间点
	AbstractState* m_state = nullptr;	// 当前状态
};
