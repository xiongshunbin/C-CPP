#pragma once

#include <QObject>
#include <QTimer>
#include "Robot.h"
#include "UserPlayer.h"
#include "Cards.h"
#include "PlayHand.h"
#include "CountDown.h"


struct BetRecord
{
	BetRecord()
	{
		reset();
	}
	void reset()
	{
		player = nullptr;
		bet = 0;
		times = 0;
	}
	Player* player;
	int bet;
	int times;	//	�ڼ��νе���
};

class GameControl : public QObject
{
	Q_OBJECT
public:
	// ��Ϸ״̬
	enum GameStatus
	{
		DispatchCard,		// ����״̬
		CallingLord,		// �е���״̬
		PlayingHand			// ����״̬
	};

	// ���״̬
	enum PlayerStatus
	{
		ThinkingForCallLord,	// ���ǽе���
		ThinkingForPlayHand,	// ���ǳ���
		winning					// ��ʤ
	};

	explicit GameControl(QObject* parent = nullptr);

	// ��ʼ�����
	void playerInit();

	// �õ���ҵ�ʵ������
	Robot* getLeftRobot();
	Robot* getRightRobot();
	UserPlayer* getUserPlayer();

	// ��ǰ���
	void setCurrentPlayer(Player* player);
	Player* getCurrentPlayer();

	// �õ�������Һʹ������
	Player* getPendPlayer();
	Cards getPendCards();

	void setPendPlayer(Player* player);
	void setPendCards(Cards cards);

	// ��ʼ���˿���
	void initAllCards();

	// ÿ�η�һ���˿���
	Card takeOneCard();

	// �õ��������ŵ���
	Cards getSurplusCards();

	// ���ÿ�������
	void resetCardData();

	// ׼���е���
	void startLordCard();

	// ��Ϊ����
	void becomeLord(Player* player, int bet);

	// ���������ҵĵ÷�
	void clearPlayerScore();

	// �õ������ע����߷���
	int getPlayerMaxBet();

private slots:
	// ����е���
	void onGrabBet(Player* player, int bet);

	// �������
	void onPlayHand(Player* player, Cards& cards);


signals:
	void playerStatusChanged(Player *player, PlayerStatus status);

	// ֪ͨ�����������������
	void notifyGrabLordBet(Player* player, int bet, bool isFirst);

	// ��Ϸ״̬�仯
	void gameStatusChanged(GameStatus status);

	// ֪ͨ��������ҳ�����
	void notifyPlayHand(Player* player, Cards& cards);

	// ����Ҵ��ݳ�������
	void pendingInfo(Player* player, Cards& cards);

private:
	Robot* m_robotLeft = nullptr;
	Robot* m_robotRight = nullptr;
	UserPlayer* m_user = nullptr;
	Player* m_currPlayer = nullptr;
	Player* m_pendPlayer = nullptr;
	Cards m_pendCards;
	Cards m_allCards;
	BetRecord m_betRecord;
	int m_curBet = 0;

};

