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

    // 初始化游戏控制类信息
    void gameControlInit();

    // 更新分数面板的分数
    void updatePlayerScore();

    // 切割并存储图片
    void initCardMap();

    // 裁剪图片
    void cropImage(QPixmap &pix, int x, int y, Card &c);

    // 初始化游戏按钮组
    void initButtonsGroup();

    // 初始化玩家在窗口中的上下文环境
    void initPlayerContext();

    // 初始化游戏场景
    void initGameScene();

    // 处理游戏的状态
    void gameStatusPrecess(GameControl::GameStatus status);

    // 发牌
    void startDispatchCard();

    // 移动扑克牌
    void cardMoveStep(Player* player, int curPos);

    // 处理分发得到的扑克牌
    void disposeCard(Player* player, const Cards& cards);

    // 更新扑克牌在窗口中的显示
    void updatePlayerCards(Player *player);

    // 显示特效动画
    void showAnimation(AnimationType type, int bet = 0);

    // 处理玩家的出牌
    void onDisposePlayHand(Player* player, Cards& cards);

    // 隐藏玩家打出的牌
    void hidePlayerDropCards(Player* player);

    // 加载玩家头像
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    // 显示玩家的最终分数
    void showEndingScorePanel();

    // 初始化闹钟倒计时
    void initCountDown();

private slots:
    // 定时器的处理动作
    void onDispatchCard();

    // 处理玩家状态的变化
    void onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status);

    // 处理玩家抢地主
    void onGrabLordBet(Player *player, int bet, bool isFirst);

    // 处理玩家选牌
    void onCardSelected(Qt::MouseButton button);

    // 处理用户玩家出牌
    void onUserPlayHand();

    // 用户玩家放弃出牌
    void onUserPass();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    enum CardAlign{Horizontal, Vertical};
    struct playerContext
    {
        // 1.玩家扑克牌显示的区域
        QRect cardRect;
        // 2.出牌的区域
        QRect playHandRect;
        // 3.扑克牌的对齐方式(水平、垂直)
        CardAlign align;
        // 4.扑克牌显示正面还是显示背面
        bool isFrontSide;
        // 5.游戏过程中的出牌信息
        QLabel* info;
        // 6.玩家的头像
        QLabel* roleImg;
        // 7.玩家刚打出的牌
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
