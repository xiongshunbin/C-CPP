#pragma once

#include "Player.h"
#include <QObject>

class Robot : public Player
{
	Q_OBJECT
public:
	using Player::Player;			// 使用继承构造函数
	explicit Robot(QObject* parent = nullptr);

	virtual void prepareCallLord() override;
	virtual void preparePlayHand() override;

	// 考虑叫地主
	virtual void thinkCallLord() override;

	virtual void thinkPlayHand() override;
};