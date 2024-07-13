#include "RobotPlayHand.h"

RobotPlayHand::RobotPlayHand(Player* player, QObject* parent)
{
	m_player = player;
}

void RobotPlayHand::run()
{
	msleep(2000);
	m_player->thinkPlayHand();
}
