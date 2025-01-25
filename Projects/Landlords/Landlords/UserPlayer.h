#pragma once

#include "Player.h"
#include <QObject>

class UserPlayer : public Player
{
	Q_OBJECT
public:
	using Player::Player;		// 使用继承构造函数
	explicit UserPlayer(QObject* parent = nullptr);

	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;

signals:
	void startCountDown();
};

