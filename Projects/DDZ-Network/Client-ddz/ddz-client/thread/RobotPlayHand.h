#pragma once

#include <QThread>
#include "Player.h"

class RobotPlayHand : public QThread
{
	Q_OBJECT
public:
	explicit RobotPlayHand(Player* player, QObject* parent = nullptr);

protected:
	void run() override;

private:
	Player* m_player;
	
};

