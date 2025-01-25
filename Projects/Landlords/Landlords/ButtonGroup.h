#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QVector>
#include "MyButton.h"

class ButtonGroup : public QWidget
{
	Q_OBJECT
public:
	enum Panel{Start, PlayCard, PassOrPlay, CallLord, Empty};
	explicit ButtonGroup(QWidget* parent = nullptr);
	~ButtonGroup();

	// 分别初始化站窗口中各个窗口
	void initStartPage();
	void initPlayCardPage();
	void initPassOrPlayPage();
	void initCallLordPage();
	void initNullPage();

	// 初始化按钮
	void initButtons();

	// 处理Page页的切换
	void selectPanel(Panel type, int bet = 0);

signals:
	// 开始游戏
	void startGame();

	// 出牌
	void playHand();

	// 不出牌
	void pass();

	// 抢地主
	void betPoint(int bet);

private:
	QStackedWidget* stackedWidget;

	QWidget* startPage;
	MyButton* btn_start;

	QWidget* playCardPage;
	MyButton* btn_playCard;

	QWidget* passOrPlayPage;
	MyButton* btn_pass;
	MyButton* btn_playCard1;

	QWidget* callLordPage;
	MyButton* btn_giveUp;
	MyButton* btn_oneScore;
	MyButton* btn_twoScore;
	MyButton* btn_threeScore;

	QWidget* nullPage;
};

