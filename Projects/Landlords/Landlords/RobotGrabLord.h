#pragma once

#include <QThread>
#include "Player.h"

class RobotGrabLord : public QThread
{
	Q_OBJECT
public:
	explicit RobotGrabLord(Player* player, QObject* parent = nullptr);

protected:
	void run() override;

private:
	Player* m_player;
};