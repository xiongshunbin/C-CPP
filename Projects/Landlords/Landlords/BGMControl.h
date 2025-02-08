#pragma once

#include <QObject>
#include <QMediaplayer>
#include <QMediaplaylist>
#include <QVector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QTimer>
#include "Cards.h"
#include "PlayHand.h"

class BGMControl : public QObject
{
	Q_OBJECT
public:
	enum RoleSex{Man, Woman};
    enum CardType
    {
        // ������
        Three,
        Foue,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace,
        Tow,
        SmallJoker,
        BigJoker,
        // ������
        Three_Double,
        Foue_Double,
        Five_Double,
        Six_Double,
        Seven_Double,
        Eight_Double,
        Nine_Double,
        Ten_Double,
        Jack_Double,
        Queen_Double,
        King_Double,
        Ace_Double,
        Tow_Double,
        // ������
        Three_Triple,
        Foue_Triple,
        Five_Triple,
        Six_Triple,
        Seven_Triple,
        Eight_Triple,
        Nine_Triple,
        Ten_Triple,
        Jack_Triple,
        Queen_Triple,
        King_Triple,
        Ace_Triple,
        Tow_Triple,
        // �������
        Plane,          // �ɻ�
        SequencePair,   // ����
        ThreeBindOne,   // ����һ
        ThreeBindPair,  // ����һ��
        Sequence,       // ˳��
        FourBindTwo,    // �Ĵ���(����)
        FourBind2Pair,  // �Ĵ�����
        Bomb,           // ը��
        JokerBomb,      // ��ը
        Pass1,          // ��
        Pass2,
        Pass3,
        Pass4,
        MoreBiger1,     // ����
        MoreBiger2,
        Biggest,        // ѹ��
        // ������
        NoOrder,        // ����
        NoRob,          // ����
        Order,          // �е���
        Rob1,           // ������
        Rob2,
        Last1,          // ֻʣ1����
        Last2           // ֻʣ2����
    };

    enum AssistMusic
    {
        Dispatch,       // ����
        SelectCard,     // ѡ��
        PlaneVoice,     // �ɻ�
        BombVoice,      // ը��
        Alert           // ����
    };

	explicit BGMControl(QObject* parent = nullptr);

	// ��ʼ�������б�
	void initPlayList();

	// 1. ��������(���ź�ֹͣ����)
	void startBGM(int volume);
	void stopBGM();
	// 2. �����������������
	void playerRobLordMusic(int point, RoleSex sex, bool isFirst);
	// 3. ���ų��Ƶı�������
    void playCardMusic(Cards cards, bool isFirst, RoleSex sex);
    void playLastMusic(CardType type, RoleSex sex);
	// 4. ���Ų����Ƶı�������
    void playPassMusic(RoleSex sex);
	// 5. ���Ÿ�������
    void playAssistMusic(AssistMusic type);
    void stopAssistMusic();
	// 6. ���Ž�β����
    void playEndingMusic(bool isWin);

private:
	// 0.man 1.woman 2.bgm 3.�������� 4.��������
	QVector<QMediaPlayer*> m_players;
	QVector<QMediaPlaylist*> m_lists;
};

