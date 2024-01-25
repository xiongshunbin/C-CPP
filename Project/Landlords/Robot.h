#pragma once

#include "Player.h"
#include <QObject>

class Robot : public Player
{
	Q_OBJECT
public:
	using Player::Player;			// ʹ�ü̳й��캯��
	explicit Robot(QObject* parent = nullptr);

	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;

	// ���ǽе���
	virtual void thinkCallLord() override;

	virtual void thinkPlayHand() override;
};