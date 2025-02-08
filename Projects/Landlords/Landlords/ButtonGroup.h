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

	// �ֱ��ʼ��վ�����и�������
	void initStartPage();
	void initPlayCardPage();
	void initPassOrPlayPage();
	void initCallLordPage();
	void initNullPage();

	// ��ʼ����ť
	void initButtons();

	// ����Pageҳ���л�
	void selectPanel(Panel type, int bet = 0);

signals:
	// ��ʼ��Ϸ
	void startGame();

	// ����
	void playHand();

	// ������
	void pass();

	// ������
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

