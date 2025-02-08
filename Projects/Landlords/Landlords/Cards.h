#pragma once

#include "Card.h"
#include <QSet>

class Cards
{
public:
	enum SortType{Asc, Desc, NoSort};

	Cards();
	explicit Cards(const Card& card);

	// ����˿���
	void add(const Card& card);
	void add(const Cards& cards);
	void add(const QVector<Cards>& cards);

	// һ���Բ���������(����������)
	Cards& operator<<(const Card& card);
	Cards& operator<<(const Cards& cards);

	// ɾ���˿���
	void remove(const Card& card);
	void remove(const Cards& cards);
	void remove(const QVector<Cards>& cards);

	// �˿��Ƶ�����
	int cardCount();

	// �Ƿ�Ϊ��
	bool isEmpty();

	// ����˿���
	void clear();

	// ������
	Card::CardPoint maxPoint();

	// ��С����
	Card::CardPoint minPoint();

	// ָ���������˿��Ƶ�����
	int pointCount(Card::CardPoint point);

	// ĳ�����Ƿ��ڼ�����
	bool contains(const Card& card);
	bool contains(const Cards& cards);

	// ���ȡ��һ���˿���
	Card takeRandCard();

	// QVecotr<Card>
	// QSet -> QVector
	CardList toCardList(SortType type = Desc);
	
private:
	QSet<Card> m_cards;
};

