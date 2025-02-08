#pragma once

#include "Cards.h"
#include "Card.h"
#include <QVector>


class PlayHand
{
public:
	// ������ϻ��߷�ʽ
	enum HandType
	{
		Hand_Unknown,					// δ֪
		Hand_Pass,						// ��

		Hand_Single,					// ��
		Hand_Pair,						// ˫

		Hand_Triple,					// ����
		Hand_Triple_Single,				// ����һ
		Hand_Triple_Pair,				// ������

		Hand_Plane,						// �ɻ�, 555_666
		Hand_Plane_Two_Single,			// �ɻ�������	555_666_3_4
		Hand_Plane_Two_Pair,			// �ɻ���˫�� 555_666_33_44

		Hand_Seq_Pair,					// ���ԣ� 33_44_55(_66...)
		Hand_Seq_Single,				// ˳�ӣ� 34567(8...)

		Hand_Bomb,						// ը��
		Hand_Bomb_Single,				// ը����һ��
		Hand_Bomb_Pair,					// ը����һ��
		Hand_Bomb_Two_Single,			// ը��������

		Hand_Bomb_Jokers,				// ��ը
		Hand_Bomb_Jokers_Single,		// ��ը��һ��
		Hand_Bomb_Jokers_Pair,			// ��ը��һ��
		Hand_Bomb_Jokers_Two_Single		// ��ը������
	};

	PlayHand();

	// ���ݸ���һ���ƣ� ͨ������������ͣ� ������ �Լ���صĸ�����Ϣ(����˳�ӣ���¼�Ƶ�����)
	explicit PlayHand(Cards& cards);
	PlayHand(HandType type, Card::CardPoint pt, int extra);

	// �õ��Ƶ�������Ϣ
	HandType getHandType() const;
	Card::CardPoint getCardPoint() const;
	int getExtra() const;

	// �Ƚ��Լ����ƺ������˵��Ƶ����ʹ�С
	bool canBeat(const PlayHand& other);

private:
	// 1. ���˿��ƽ��з��ࣺ 1�ŵ��ж����֣� 2�ŵ��ж����֣� 3�ŵ��ж����֣� 4�ŵ��ж�����
	void classify(Cards& cards);

	// 2. �����ͽ��з���
	void judgeCardType();

	// �ж��Ƶ�����
	bool isPass();					// ��������
	bool isSingle();				// ��
	bool isPair();					// ��
	bool isTriple();				// ����(��ͬ)
	bool isTripleSingle();			// ����һ
	bool isTriplePair();			// ������
	bool isPlane();					// �ɻ�
	bool isPlaneTwoSingle();		// �ɻ�������
	bool isPlaneTwoPair();			// �ɻ�����˫
	bool isSeqPair();				// ����
	bool isSeqSingle();				// ˳��
	bool isBomb();					// ը��
	bool isBombSingle();			// ը����һ��
	bool isBombPair();				// ը����һ��
	bool isBombTwoSingle();			// ը��������
	bool isBombJokers();			// ��ը
	bool isBombJokersSingle();		// ��ը��һ��
	bool isBombJokersPair();		// ��ը��һ��
	bool isBombJokersTwoSingle();	// ��ը������


private:
	HandType m_type;
	Card::CardPoint m_pt;
	int m_extra;
	QVector<Card::CardPoint> m_oneCard;
	QVector<Card::CardPoint> m_twoCard;
	QVector<Card::CardPoint> m_threeCard;
	QVector<Card::CardPoint> m_fourCard;
};

