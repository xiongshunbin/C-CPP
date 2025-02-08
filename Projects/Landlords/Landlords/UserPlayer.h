#pragma once

#include "Player.h"
#include <QObject>

class UserPlayer : public Player
{
	Q_OBJECT
public:
	using Player::Player;		// ʹ�ü̳й��캯��
	explicit UserPlayer(QObject* parent = nullptr);

	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;

signals:
	void startCountDown();
};

