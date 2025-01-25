#pragma once

#include <QtWidgets/QWidget>
#include "ScorePanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QRandomGenerator>
#include <QVector>
#include <QMap>
#include <QLabel>
#include <QTimer>
#include <QHash>
#include <QMouseEvent>
#include <QPropertyanimation>
#include "ButtonGroup.h"
#include "GameControl.h"
#include "CardPanel.h"
#include "AnimationWindow.h"
#include "EndingPanel.h"
#include "BGMControl.h"

class GamePanel : public QWidget
{
    Q_OBJECT

public:
    explicit GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    enum AnimationType{SeqSingle, SeqPair, Plane, JokerBomb, Bomb, Bet};

    void initWidget();

    // ��ʼ����Ϸ��������Ϣ
    void gameControlInit();

    // ���·������ķ���
    void updatePlayerScore();

    // �и�洢ͼƬ
    void initCardMap();

    // �ü�ͼƬ
    void cropImage(QPixmap &pix, int x, int y, Card &c);

    // ��ʼ����Ϸ��ť��
    void initButtonsGroup();

    // ��ʼ������ڴ����е������Ļ���
    void initPlayerContext();

    // ��ʼ����Ϸ����
    void initGameScene();

    // ������Ϸ��״̬
    void gameStatusPrecess(GameControl::GameStatus status);

    // ����
    void startDispatchCard();

    // �ƶ��˿���
    void cardMoveStep(Player* player, int curPos);

    // ����ַ��õ����˿���
    void disposeCard(Player* player, const Cards& cards);

    // �����˿����ڴ����е���ʾ
    void updatePlayerCards(Player *player);

    // ��ʾ��Ч����
    void showAnimation(AnimationType type, int bet = 0);

    // ������ҵĳ���
    void onDisposePlayHand(Player* player, Cards& cards);

    // ������Ҵ������
    void hidePlayerDropCards(Player* player);

    // �������ͷ��
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    // ��ʾ��ҵ����շ���
    void showEndingScorePanel();

    // ��ʼ�����ӵ���ʱ
    void initCountDown();

private slots:
    // ��ʱ���Ĵ�����
    void onDispatchCard();

    // �������״̬�ı仯
    void onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status);

    // �������������
    void onGrabLordBet(Player *player, int bet, bool isFirst);

    // �������ѡ��
    void onCardSelected(Qt::MouseButton button);

    // �����û���ҳ���
    void onUserPlayHand();

    // �û���ҷ�������
    void onUserPass();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    enum CardAlign{Horizontal, Vertical};
    struct playerContext
    {
        // 1.����˿�����ʾ������
        QRect cardRect;
        // 2.���Ƶ�����
        QRect playHandRect;
        // 3.�˿��ƵĶ��뷽ʽ(ˮƽ����ֱ)
        CardAlign align;
        // 4.�˿�����ʾ���滹����ʾ����
        bool isFrontSide;
        // 5.��Ϸ�����еĳ�����Ϣ
        QLabel* info;
        // 6.��ҵ�ͷ��
        QLabel* roleImg;
        // 7.��Ҹմ������
        Cards lastCards;
    };
    ScorePanel* scorePanel;
    ButtonGroup* btnGroup;
    QPixmap m_bkImage;
    GameControl* m_gameCtl;
    QVector<Player*> m_playerList;
    QMap<Card, CardPanel*> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*, playerContext> m_contextMap;
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer* m_timer;
    AnimationWindow* m_animation;
    CardPanel* m_curSelCard;
    QSet<CardPanel*> m_selectCards;
    QRect m_cardsRect;
    QHash<CardPanel*, QRect> m_userCards;
    CountDown* m_countDown;
    BGMControl* m_bgm;
};
