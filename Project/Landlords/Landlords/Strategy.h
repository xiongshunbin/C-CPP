#pragma once

#include "Player.h"
#include <QVector>
#include "PlayHand.h"

class Strategy
{
public:
	Strategy(Player* player, const Cards& cards);

	// 1. �ƶ����Ʋ���
	Cards makeStrategy();
	// 2. ��һ������firstPlay
	Cards firstPlay();
	// 3. �õ���ָ�����ʹ����
	Cards getGreaterCards(PlayHand type);
	// 4. �ܴ��ָ������ʱ���ж��ǳ��ƻ��Ƿ��У�����true->���ƣ�����false->����
	bool whetherToBeat(Cards& cs);


	// 5. �ҳ�ָ������(count)����ͬ��������(point), �ҳ�count�ŵ���Ϊpoint����
	Cards findSamePointCards(Card::CardPoint point, int count);

	// 6. �ҳ���������Ϊcount���ƣ� ===> �õ�һ�������˿�������
	QVector<Cards> findCardsByCount(int count);

	// 7. ���ݵ�����Χ����
	Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);

	// 8. ���������ƣ� ����ָ��Ҫ�ҵ����Ƿ�Ҫ���ָ��������
	QVector<Cards> findCardType(PlayHand hand, bool beat);

	// 9. ��ָ����Cards��������ѡ������������˳��
	void pickSeqSingles(QVector<QVector<Cards>>& allSeqRecord, const QVector<Cards>& seqSingle, const Cards& cards);

	// 10. ���ŵ�˳�ӵļ��ϵ�ɸѡ����
	QVector<Cards> pickOptimalSeqSingles();

private:
	using function = Cards(Strategy::*)(Card::CardPoint point);
	struct CardInfo
	{
		Card::CardPoint begin;
		Card::CardPoint end;
		int extra;				// ˳�ӻ������Ե�����
		bool beat;
		int number;				// ָ���������Ƶ�����
		int base;				// �������˳�ӻ������Ե�����
		function getSeq;
	};
	QVector<Cards> getCards(Card::CardPoint point, int number);
	QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
	QVector<Cards> getPlane(Card::CardPoint begin);
	QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
	QVector<Cards> getSeqPairOrSeqSingle(CardInfo& info);
	Cards getBaseSeqPair(Card::CardPoint point);
	Cards getBaseSeqSingle(Card::CardPoint point);
	QVector<Cards> getBomb(Card::CardPoint begin);

private:
	Player* m_player;
	Cards m_cards;
};

