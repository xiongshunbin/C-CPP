#include "GameControl.h"
#include <QRandomGenerator>

GameControl::GameControl(QObject* parent) : QObject(parent)
{
}

void GameControl::playerInit()
{
	// ����ʵ����
	m_robotLeft = new Robot(u8"������A", this);
	m_robotRight = new Robot(u8"������B", this);
	m_user = new UserPlayer(u8"���Լ�", this);

	// ͷ�����ʾ��λ
	m_robotLeft->setDirection(Player::Left);
	m_robotRight->setDirection(Player::Right);
	m_user->setDirection(Player::Right);

	// �Ա�
	Player::Sex sex;
	sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
	m_robotLeft->setSex(sex);
	sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
	m_robotRight->setSex(sex);
	sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
	m_user->setSex(sex);

	// ����˳��
	// user
	m_user->setPrevPlayer(m_robotLeft);
	m_user->setNextPlayer(m_robotRight);

	// left robot
	m_robotLeft->setPrevPlayer(m_robotRight);
	m_robotLeft->setNextPlayer(m_user);

	// right robot
	m_robotRight->setPrevPlayer(m_user);
	m_robotRight->setNextPlayer(m_robotLeft);

	// ָ����ǰ���
	m_currPlayer = m_user;

	// ������ҷ������ź�
	connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
	connect(m_robotLeft, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);
	connect(m_robotRight, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);

	// ���ݳ�����Ҷ������Ҵ������
	connect(this, &GameControl::pendingInfo, m_robotLeft, &Robot::storePendingInfo);
	connect(this, &GameControl::pendingInfo, m_robotRight, &Robot::storePendingInfo);
	connect(this, &GameControl::pendingInfo, m_user, &UserPlayer::storePendingInfo);

	// ������ҳ���
	connect(m_robotLeft, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
	connect(m_robotRight, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
	connect(m_user, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
}

Robot* GameControl::getLeftRobot()
{
	return m_robotLeft;
}

Robot* GameControl::getRightRobot()
{
	return m_robotRight;
}

UserPlayer* GameControl::getUserPlayer()
{
	return m_user;
}

void GameControl::setCurrentPlayer(Player* player)
{
	m_currPlayer = player;
}

Player* GameControl::getCurrentPlayer()
{
	return m_currPlayer;
}

Player* GameControl::getPendPlayer()
{
	return m_pendPlayer;
}

Cards GameControl::getPendCards()
{
	return m_pendCards;
}

void GameControl::setPendPlayer(Player* player)
{
	m_pendPlayer = player;
}

void GameControl::setPendCards(Cards cards)
{
	m_pendCards = cards;
}

void GameControl::initAllCards()
{
	m_allCards.clear();
	for (int p = Card::Card_Begin + 1; p < Card::Card_SJ; ++p)
	{
		for (int s = Card::Suit_Begin + 1; s < Card::Suit_End; ++s)
		{
			Card c{ (Card::CardPoint)p, (Card::CardSuit)s };
			m_allCards.add(c);
		}
	}
	m_allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
	m_allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

Card GameControl::takeOneCard()
{
	return m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards()
{
	return m_allCards;
}

void GameControl::resetCardData()
{
	// ϴ��
	initAllCards();

	// ���������ҵ���
	m_robotLeft->clearCards();
	m_robotRight->clearCards();
	m_user->clearCards();

	// ��ʼ��������Һ���
	m_pendPlayer = nullptr;
	m_pendCards.clear();
	
	// ��ʼ����ұ��������
	m_robotLeft->setPendingInfo(m_pendPlayer, m_pendCards);
	m_robotRight->setPendingInfo(m_pendPlayer, m_pendCards);
	m_user->setPendingInfo(m_pendPlayer, m_pendCards);
}

void GameControl::startLordCard()
{
	m_currPlayer->prepareCallLord();
	emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
}

void GameControl::becomeLord(Player* player, int bet)
{
	m_curBet = bet;
	player->setRole(Player::Lord);
	player->getPrevPlayer()->setRole(Player::Farmer);
	player->getNextPlayer()->setRole(Player::Farmer);

	m_currPlayer = player;
	player->storeDispatchCard(m_allCards);

	QTimer::singleShot(1000, this, [=]() {
		emit gameStatusChanged(PlayingHand);
		emit playerStatusChanged(player, ThinkingForPlayHand);
		m_currPlayer->preparePlayHand();
	});
}

void GameControl::clearPlayerScore()
{
	m_robotLeft->setScore(0);
	m_robotRight->setScore(0);
	m_user->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
	return m_betRecord.bet;
}

void GameControl::onPlayHand(Player* player, Cards& cards)
{
	// 1. ����ҳ��Ƶ��ź�ת����������
	emit notifyPlayHand(player, cards);

	// 2. �ж���Ҵ�������Ƿ��ǿ��ƣ�������ǿ��ƣ���������ҷ����źţ����������Ҷ��󼰴������
	if (!cards.isEmpty())
	{
		m_pendPlayer = player;
		m_pendCards = cards;
		emit pendingInfo(player, cards);
	}
	// �����ը�����׷ַ���
	PlayHand::HandType type = PlayHand(cards).getHandType();
	if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers)
		m_curBet *= 2;

	// 3. �����ҵ��Ƴ����ˣ����㱾����Ϸ�ܵ÷�
	if (player->getCards().isEmpty())
	{
		Player* prev = player->getPrevPlayer();
		Player* next = player->getNextPlayer();
		if (player->getRole() == Player::Lord)
		{
			player->setScore(player->getScore() + 2 * m_curBet);
			prev->setScore(prev->getScore() - m_curBet);
			next->setScore(next->getScore() - m_curBet);
			player->setWin(true);
			prev->setWin(false);
			next->setWin(false);
		}
		else
		{
			player->setWin(true);
			player->setScore(player->getScore() + m_curBet);
			if (prev->getRole() == Player::Lord)
			{
				prev->setScore(prev->getScore() - 2 * m_curBet);
				next->setScore(next->getScore() + m_curBet);
				prev->setWin(false);
				next->setWin(true);
			}
			else
			{
				next->setScore(next->getScore() - 2 * m_curBet);
				prev->setScore(prev->getScore() + m_curBet);
				next->setWin(false);
				prev->setWin(true);
			}
		}
		emit playerStatusChanged(player, winning);
		return;
	}

	// 4. ��û�г��֪꣬ͨ��һ����Ҽ�������
	m_currPlayer = player->getNextPlayer();
	m_currPlayer->preparePlayHand();
	emit playerStatusChanged(m_currPlayer, ThinkingForPlayHand);
}

void GameControl::onGrabBet(Player* player, int bet)
{
	// 1.֪ͨ��������ҽе���(������Ϣ��ʾ)
	if (bet == 0 || m_betRecord.bet >= bet)
	{
		emit notifyGrabLordBet(player, 0, false);
	}
	else if (bet > 0 && m_betRecord.bet == 0)
	{
		// ��һ�������������
		emit notifyGrabLordBet(player, bet, true);
	}
	else
	{
		// ��2��3�������������
		emit notifyGrabLordBet(player, bet, false);
	}

	// 2.�ж������ע�ǲ���3�֣�����ǣ�����������
	if (bet == 3)
	{
		// ��ҳ�Ϊ����
		becomeLord(player, bet);
		// �������
		m_betRecord.reset();

		return;
	}

	// 3.���������ע����3�֣��������ע�ķ������бȽϣ������ߵ��ǵ���
	if (m_betRecord.bet < bet)
	{
		m_betRecord.bet = bet;
		m_betRecord.player = player;
	}
	m_betRecord.times++;

	// ���ÿ����Ҷ�����һ�ε���������������
	if (m_betRecord.times >= 3)
	{
		if (m_betRecord.bet == 0)
			emit gameStatusChanged(DispatchCard);
		else
			becomeLord(m_betRecord.player, m_betRecord.bet);

		m_betRecord.reset();
		return;
	}

	// 4.�л���ң�֪ͨ��һ����Ҽ���������
	m_currPlayer = player->getNextPlayer();
	
	// �����źŸ������棬��֪��ǰ״̬Ϊ������
	emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
	m_currPlayer->prepareCallLord();
}
