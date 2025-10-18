#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(QWidget* parent) : QWidget(parent)
{
    stackedWidget = new QStackedWidget(this);

    initStartPage();
    initPlayCardPage();
    initPassOrPlayPage();
    initCallLordPage();
    initNullPage();

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(stackedWidget);

    setLayout(layout);
}

ButtonGroup::~ButtonGroup()
{
}

void ButtonGroup::initButtons()
{
    // 开始游戏
    btn_start->setImage(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");

    // 出牌
    btn_playCard->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    btn_playCard1->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");

    // 不要
    btn_pass->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");

    // 不抢
    btn_giveUp->setImage(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");

    // 1、2、3分
    btn_oneScore->setImage(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    btn_twoScore->setImage(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    btn_threeScore->setImage(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");

    // 设置按钮的大小
    QVector<MyButton*> btns;
    btns << btn_start << btn_playCard << btn_playCard1 << btn_pass << btn_giveUp << btn_oneScore << btn_twoScore << btn_threeScore;

    for(int i = 0; i < btns.size(); i ++)
        btns[i]->setFixedSize(90, 45);

    connect(btn_start, &MyButton::clicked, this, &ButtonGroup::startGame);
    connect(btn_playCard, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(btn_playCard1, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(btn_pass, &MyButton::clicked, this, &ButtonGroup::pass);
    connect(btn_giveUp, &MyButton::clicked, this, [=]() {
        emit betPoint(0);
        });
    connect(btn_oneScore, &MyButton::clicked, this, [=]() {
        emit betPoint(1);
        });
    connect(btn_twoScore, &MyButton::clicked, this, [=]() {
        emit betPoint(2);
        });
    connect(btn_threeScore, &MyButton::clicked, this, [=]() {
        emit betPoint(3);
        });
}

void ButtonGroup::initStartPage()
{
    startPage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    btn_start = new MyButton;
    layout->addStretch();
    layout->addWidget(btn_start);
    layout->addStretch();
    startPage->setLayout(layout);

    stackedWidget->addWidget(startPage);
}

void ButtonGroup::initPlayCardPage()
{
    playCardPage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    btn_playCard = new MyButton;
    layout->addStretch();
    layout->addWidget(btn_playCard);
    layout->addStretch();
    playCardPage->setLayout(layout);

    stackedWidget->addWidget(playCardPage);
}

void ButtonGroup::initPassOrPlayPage()
{
    passOrPlayPage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    btn_pass = new MyButton;
    btn_playCard1 = new MyButton;
    layout->addStretch();
    layout->addWidget(btn_pass);
    layout->addItem(new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(btn_playCard1);
    layout->addStretch();
    passOrPlayPage->setLayout(layout);

    stackedWidget->addWidget(passOrPlayPage);
}

void ButtonGroup::initCallLordPage()
{
    callLordPage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    btn_giveUp = new MyButton;
    btn_oneScore = new MyButton;
    btn_twoScore = new MyButton;
    btn_threeScore = new MyButton;
    layout->addStretch();
    layout->addWidget(btn_giveUp);
    layout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(btn_oneScore);
    layout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(btn_twoScore);
    layout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(btn_threeScore);
    layout->addStretch();
    callLordPage->setLayout(layout);

    stackedWidget->addWidget(callLordPage);
}

void ButtonGroup::initNullPage()
{
    nullPage = new QWidget;

    stackedWidget->addWidget(nullPage);
}

void ButtonGroup::selectPanel(Panel type, int bet)
{
    stackedWidget->setCurrentIndex(type);
    if (type != CallLord)    return;
    if (bet == 0)
    {
        btn_oneScore->setVisible(true);
        btn_twoScore->setVisible(true);
        btn_threeScore->setVisible(true);
    }
    else if(bet == 1)
    {
        btn_oneScore->setVisible(false);
        btn_twoScore->setVisible(true);
        btn_threeScore->setVisible(true);
    }
    else if (bet == 2)
    {
        btn_oneScore->setVisible(false);
        btn_twoScore->setVisible(false);
        btn_threeScore->setVisible(true);
    }
}
