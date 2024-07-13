#pragma once
#include <QObject>
#include <QString>
#include "Cards.h"

class Player : public QObject
{
	Q_OBJECT
public:
	enum Role{Lord, Farmer};				// ��ɫ
	enum Sex{Man, Woman};					// �Ա�
	enum Direction{Left, Right};			// ͷ�����ʾ��λ
	enum Type{Robot, User, UnKnow};			// ��ҵ�����

	explicit Player(QObject* parent = nullptr);
	explicit Player(QString name, QObject* parent = nullptr);

	// ����
	void setName(QString name);
	QString getName();

	// ��ɫ
	void setRole(Role role);
	Role getRole();

	// �Ա�
	void setSex(Sex sex);
	Sex getSex();

	// ��λ
	void setDirection(Direction direction);
	Direction getDirection();

	// �������
	void setType(Type type);
	Type getType();

	// ��ҷ���
	void setScore(int score);
	int getScore();

	// ��Ϸ���
	void setWin(bool flag);
	bool isWin();

	// �ṩ��ǰ������ϼ�/�¼Ҷ���
	void setPrevPlayer(Player* player);
	void setNextPlayer(Player* player);
	Player* getPrevPlayer();
	Player* getNextPlayer();

	// �е���
	void grabLordBet(int point);

	// �洢�˿���(����)
	void storeDispatchCard(const Card& card);
	void storeDispatchCard(const Cards& cards);

	// �õ����е��˿���
	Cards getCards();

	// ���������е������˿���
	void clearCards();

	// ����
	void playHand(Cards& cards);

	// ���ó��Ƶ�����Լ���������˿���
	void setPendingInfo(Player *player, Cards& cards);
	Player* getPendPlayer();
	Cards getPendCards();

	// �麯��
	virtual void prepareCallLord();
	virtual void preparePlayHand();
	virtual void thinkCallLord();
	virtual void thinkPlayHand();

signals:
	// ֪ͨ�Ѿ��е�����ע
	void notifyGrabLordBet(Player* player, int bet);

	// ֪ͨ�Ѿ�����
	void notifyPlayHand(Player* player, Cards& cards);

	// ֪ͨ��Ϊ�������Ѿ�������
	void notifyPickCards(Player* player, const Cards& cards);
	

public slots:
	// �洢������Ҷ���ʹ������
	void storePendingInfo(Player* player, const Cards& cards);

protected:
	QString m_name;
	int m_score = 0;
	Role m_role;
	Sex m_sex;
	Direction m_direction;
	Type m_type;
	bool m_isWin = false;
	Player* m_prev = nullptr;
	Player* m_next = nullptr;
	Cards m_cards;		// �洢�����˿���(������е���)
	Cards m_pendCards;
	Player* m_pendPlayer = nullptr;
};

