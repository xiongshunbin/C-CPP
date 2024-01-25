#include "GamePanel.h"

GamePanel::GamePanel(QWidget *parent) : QWidget(parent)
{
	initWidget();

	// 1.����ͼ
	int num = QRandomGenerator::global()->bounded(10);
	QString path = QString(":/images/background-%1.png").arg(num + 1);
	m_bkImage.load(path);

	// 2.���ڵı����Լ���С
	this->setWindowTitle(u8"���ֶ�����");
	this->setFixedSize(1000, 650);

	// 3.ʵ������Ϸ������
	gameControlInit();

	// 4.��ʼ����ҵ÷�(����)
	updatePlayerScore();

	// 5.�и���Ϸ�е�ͼƬ
	initCardMap();

	// 6.��ʼ����Ϸ�еİ�ť��
	initButtonsGroup();

	// 7.��ʼ������ڴ����е������Ļ���
	initPlayerContext();

	// 8.�˿��Ƴ�����ʼ��
	initGameScene();

	// 9.����ʱ���ڳ�ʼ��
	initCountDown();

	// ��ʱ��ʵ����
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);

	m_animation = new AnimationWindow(this);
	m_bgm = new BGMControl(this);

	setWindowIcon(QIcon(":/images/logo.ico"));
}

GamePanel::~GamePanel()
{}

void GamePanel::initWidget()
{
	QVBoxLayout* VLayout = new QVBoxLayout;
	VLayout->setMargin(0);
	scorePanel = new ScorePanel;

	QWidget* topWidget = new QWidget(this);
	QHBoxLayout* HLayout = new QHBoxLayout;
	HLayout->setMargin(0);
	topWidget->setLayout(HLayout);
	HLayout->addStretch();
	HLayout->addWidget(scorePanel);

	VLayout->addWidget(topWidget);
	VLayout->addStretch();

	btnGroup = new ButtonGroup(this);
	VLayout->addWidget(btnGroup);
	VLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Fixed, QSizePolicy::Fixed));

	setLayout(VLayout);
}

void GamePanel::gameControlInit()
{
	m_gameCtl = new GameControl(this);
	m_gameCtl->playerInit();
	// �õ�������ҵ�ʵ������
	Robot* leftRobot = m_gameCtl->getLeftRobot();
	Robot* rightRobot = m_gameCtl->getRightRobot();
	UserPlayer* user = m_gameCtl->getUserPlayer();
	// �洢��˳���������ˣ��Ҳ�����ˣ���ǰ���
	m_playerList << leftRobot << rightRobot << user;

	connect(m_gameCtl, &GameControl::playerStatusChanged, this, &GamePanel::onPlayerStatusChanged);
	connect(m_gameCtl, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
	connect(m_gameCtl, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
	connect(m_gameCtl, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

	connect(leftRobot, &Player::notifyPickCards, this, &GamePanel::disposeCard);
	connect(rightRobot, &Player::notifyPickCards, this, &GamePanel::disposeCard);
	connect(user, &Player::notifyPickCards, this, &GamePanel::disposeCard);
}

void GamePanel::updatePlayerScore()
{
	scorePanel->setScores(
		m_playerList[0]->getScore(), 
		m_playerList[1]->getScore(), 
		m_playerList[2]->getScore());
}

void GamePanel::initCardMap()
{
	// 1.���ش�ͼ
	QPixmap pixmap(":/images/card.png");

	// 2.����ÿ��ͼƬ��С
	m_cardSize.setWidth(pixmap.width() / 13);
	m_cardSize.setHeight(pixmap.height() / 5);

	// 3.����ͼ
	m_cardBackImg = pixmap.copy(2 * m_cardSize.width(), 4 * m_cardSize.height(), 
								m_cardSize.width(), m_cardSize.height());

	//   ������ɫ
	for (int i = 0, suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++i, ++suit)
	{
		for (int j = 0, point = Card::Card_Begin + 1; point < Card::Card_SJ; ++j, ++point)
		{
			Card card((Card::CardPoint)point, (Card::CardSuit)suit);
			// �ü�ͼƬ
			cropImage(pixmap, j * m_cardSize.width(), i * m_cardSize.height(), card);
		}
	}
	//   ��С��
	Card c;
	c.setPoint(Card::Card_SJ);
	c.setSuit(Card::Suit_Begin);
	cropImage(pixmap, 0, 4 * m_cardSize.height(), c);

	c.setPoint(Card::Card_BJ);
	cropImage(pixmap, m_cardSize.width(), 4 * m_cardSize.height(), c);
}

void GamePanel::cropImage(QPixmap& pix, int x, int y, Card& c)
{
	QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
	CardPanel* panel = new CardPanel(this);
	panel->setImage(sub, m_cardBackImg);
	panel->setCard(c);
	panel->hide();
	m_cardMap.insert(c, panel);
	connect(panel, &CardPanel::cardSelected, this, &GamePanel::onCardSelected);
}

void GamePanel::initButtonsGroup()
{
	btnGroup->initButtons();
	btnGroup->selectPanel(ButtonGroup::Start);

	connect(btnGroup, &ButtonGroup::startGame, this, [=]() {
		// ����ĳ�ʼ��
		btnGroup->selectPanel(ButtonGroup::Empty);
		m_gameCtl->clearPlayerScore();
		updatePlayerScore();
		// �޸���Ϸ״̬ -> ����״̬
		gameStatusPrecess(GameControl::DispatchCard);
		// ���ű�������
		m_bgm->startBGM(80);
	});
	connect(btnGroup, &ButtonGroup::playHand, this, &GamePanel::onUserPlayHand);
	connect(btnGroup, &ButtonGroup::pass, this, &GamePanel::onUserPass);
	connect(btnGroup, &ButtonGroup::betPoint, this, [=](int bet) {
		m_gameCtl->getUserPlayer()->grabLordBet(bet);
		btnGroup->selectPanel(ButtonGroup::Empty);
	});
}

void GamePanel::initPlayerContext()
{
	// 1.��������˿��Ƶ�����
	const QRect cardsRect[] =
	{
		// x, y, width, height
		QRect(90, 130, 100, height() - 200),                    // ��������
		QRect(rect().right() - 190, 130, 100, height() - 200),  // �Ҳ������
		QRect(250, rect().bottom() - 120, width() - 500, 100)   // ��ǰ���
	};
	// 2.��ҳ��Ƶ�����
	const QRect playHandRect[] =
	{
		QRect(260, 150, 100, 100),                              // ��������
		QRect(rect().right() - 360, 150, 100, 100),             // �Ҳ������
		QRect(150, rect().bottom() - 290, width() - 300, 105)   // ��ǰ���
	};
	// 3.���ͷ����ʾ��λ��
	const QPoint roleImgPos[] =
	{
		QPoint(cardsRect[0].left() - 80, cardsRect[0].height() / 2 + 20),     // ��������
		QPoint(cardsRect[1].right() + 10, cardsRect[1].height() / 2 + 20),    // �Ҳ������
		QPoint(cardsRect[2].right() - 10, cardsRect[2].top() - 10)            // ��ǰ���
	};

	// ѭ��
	int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
	for (int i = 0; i < m_playerList.size(); i++)
	{
		playerContext context;
		context.align = (i == index) ? Horizontal : Vertical;
		context.isFrontSide = (i == index) ? true : false;
		context.cardRect = cardsRect[i];
		context.playHandRect = playHandRect[i];

		// ��ʾ��Ϣ
		context.info = new QLabel(this);
		context.info->resize(160, 98);
		context.info->hide();
		// ��ʾ���������������λ��
		QRect rect = playHandRect[i];
		QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
				  rect.top() + (rect.height() - context.info->height()) / 2);
		context.info->move(pt);
		// ��ҵ�ͷ��
		context.roleImg = new QLabel(this);
		context.roleImg->resize(84, 120);
		context.roleImg->hide();
		context.roleImg->move(roleImgPos[i]);

		m_contextMap.insert(m_playerList.at(i), context);
	}
}

void GamePanel::initGameScene()
{
	// 1.���������˿���
	m_baseCard = new CardPanel(this);
	m_baseCard->setImage(m_cardBackImg, m_cardBackImg);

	// 2.���ƹ������ƶ����˿���
	m_moveCard = new CardPanel(this);
	m_moveCard->setImage(m_cardBackImg, m_cardBackImg);

	// 3.�������ŵ���(���ڴ��ڵ���ʾ)
	for (int i = 0; i < 3; i++)
	{
		CardPanel* panel = new CardPanel(this);
		panel->setImage(m_cardBackImg, m_cardBackImg);
		panel->hide();
		m_last3Card.push_back(panel);
	}

	// �˿��Ƶ�λ��
	m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
						   height() / 2 - 100);
	m_baseCard->move(m_baseCardPos);
	m_moveCard->move(m_baseCardPos);

	int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;
	for (int i = 0; i < 3; i++)
		m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);

}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
	// ��¼��Ϸ״̬
	m_gameStatus = status;

	// ������Ϸ״̬
	switch (status)
	{
	case GameControl::DispatchCard:
		startDispatchCard();
		break;
	case GameControl::CallingLord:
	{
		// ȡ�����Ƶ�����
		CardList last3Card = m_gameCtl->getSurplusCards().toCardList();

		// �����ƴ�������ͼƬ
		for (int i = 0; i < last3Card.size(); i++)
		{
			QPixmap front = m_cardMap[last3Card[i]]->getImage();
			m_last3Card[i]->setImage(front, m_cardBackImg);
			m_last3Card[i]->hide();
		}
		// ��ʼ�е���
		m_gameCtl->startLordCard();
		break;
	}
	case GameControl::PlayingHand:
		// ���ط������ĵ��ƺ��ƶ�����
		m_baseCard->hide();
		m_moveCard->hide();
		// ��ʾ�������������ŵ���
		for (int i = 0; i < m_last3Card.size(); i++)
			m_last3Card.at(i)->show();
		
		for (int i = 0; i < m_playerList.size(); i++)
		{
			playerContext& context = m_contextMap[m_playerList[i]];
			// ���ظ�����������������е���ʾ��Ϣ
			context.info->hide();
			// ��ʾ������ҵ�ͷ��
			Player* player = m_playerList.at(i);
			QPixmap pixmap = loadRoleImage(player->getSex(), player->getDirection(), player->getRole());
			context.roleImg->setPixmap(pixmap);
			context.roleImg->show();
		}
		break;
	default:
		break;
	}
}

void GamePanel::startDispatchCard()
{
	// ����ÿ�ſ��Ƶ�����
	for (auto it = m_cardMap.begin(); it != m_cardMap.end(); it ++)
	{
		it.value()->setSelected(false);
		it.value()->setFrontSide(true);
		it.value()->hide();
	}
	// �������ŵ���
	for (int i = 0; i < m_last3Card.size(); i ++)
		m_last3Card[i]->hide();

	// ��������ڴ����е���������Ϣ
	int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
	for (int i = 0; i < m_playerList.size(); i ++)
	{
		m_contextMap[m_playerList.at(i)].lastCards.clear();
		m_contextMap[m_playerList.at(i)].info->hide();
		m_contextMap[m_playerList.at(i)].roleImg->hide();
		m_contextMap[m_playerList.at(i)].isFrontSide = (i == index) ? true : false;
	}

	// ��ʾ����
	m_baseCard->show();

	// ���ذ�ť���
	btnGroup->selectPanel(ButtonGroup::Empty);

	// ����������ҵĿ�������
	m_gameCtl->resetCardData();
	
	// ������ʱ��
	m_timer->start(10);
	
	// ���ű�������
	m_bgm->playAssistMusic(BGMControl::Dispatch);
}

void GamePanel::onDispatchCard()
{
	// ��¼�˿��Ƶ�λ��
	static int curMovePos = 0;

	// ��ǰ���
	Player* curPlayer = m_gameCtl->getCurrentPlayer();

	if (curMovePos >= 100)
	{
		// ����ҷ�һ����
		Card card = m_gameCtl->takeOneCard();
		curPlayer->storeDispatchCard(card);
		Cards cs(card);
		// disposCard(curPlayer, cs);

		// �л����
		m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
		curMovePos = 0;
		curPlayer = m_gameCtl->getCurrentPlayer();

		// �ж����Ƿ�����
		if (m_gameCtl->getSurplusCards().cardCount() == 3)
		{
			// ��ֹ��ʱ��
			m_timer->stop();
			// �л���Ϸ״̬
			gameStatusPrecess(GameControl::CallingLord);
			// ��ֹ�������ֵĲ���
			m_bgm->stopAssistMusic();
		}
	}

	// �ƶ��˿���
	cardMoveStep(curPlayer, curMovePos);
	curMovePos += 15;
}

void GamePanel::cardMoveStep(Player* player, int curPos)
{
	// �õ�ÿ������˿��Ƶ�չʾ����
	QRect cardRect = m_contextMap[player].cardRect;

	// ÿ����ҵĵ�Ԫ����
	const int unit[] = {
		(m_baseCardPos.x() - cardRect.right()) / 100,
		(cardRect.left() - m_baseCardPos.x()) / 100,
		(cardRect.top() - m_baseCardPos.y()) / 100
	};
	
	// ÿ�δ����ƶ���ʱ��ÿ����Ҷ�Ӧ���Ƶ�ʱʱ����λ��
	const QPoint pos[] = {
		QPoint(m_baseCardPos.x() - curPos * unit[0], m_baseCardPos.y()),
		QPoint(m_baseCardPos.x() + curPos * unit[1], m_baseCardPos.y()),
		QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos * unit[2])
	};

	// �ƶ��˿��ƴ���
	int index = m_playerList.indexOf(player);
	m_moveCard->move(pos[index]);

	// �ٽ�״̬����
	if (curPos == 0)
		m_moveCard->show();

	if (curPos == 100)
		m_moveCard->hide();
}

void GamePanel::disposeCard(Player* player, const Cards& cards)
{
	Cards& myCard = const_cast<Cards&>(cards);
	CardList list = myCard.toCardList();
	for (int i = 0; i < list.size(); i ++)
	{
		CardPanel* panel = m_cardMap[list.at(i)];
		panel->setOwner(player);
	}

	// �����˿����ڴ����е���ʾ
	updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player* player)
{
	Cards cards = player->getCards();
	CardList list = cards.toCardList();

	m_cardsRect = QRect();
	m_userCards.clear();

	// ȡ��չʾ�˿��Ƶ�����
	QRect cardsRect = m_contextMap[player].cardRect;
	int cardSpace = 20;

	for (int i = 0; i < list.size(); i ++)
	{
		CardPanel* panel = m_cardMap[list.at(i)];
		panel->show();
		panel->raise();
		panel->setFrontSide(m_contextMap[player].isFrontSide);

		// ˮƽ����ֱչʾ
		if (m_contextMap[player].align == Horizontal)
		{
			int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
			int topY = cardsRect.top() + (cardsRect.height() - panel->height()) / 2;
			if (panel->isSelected())
				topY -= 10;
			panel->move(leftX + cardSpace * i, topY);
			m_cardsRect = QRect(leftX, topY, i * cardSpace + m_cardSize.width(), m_cardSize.height());
			int curWidth = 0;
			if (list.size() - 1 == i)
				curWidth = m_cardSize.width();
			else
				curWidth = cardSpace;
			QRect cardRect(leftX + cardSpace * i, topY, curWidth, m_cardSize.height());
			m_userCards.insert(panel, cardRect);
		}
		else
		{
			int leftX = cardsRect.left() + (cardsRect.width() - panel->width()) / 2;
			int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
			panel->move(leftX, topY + i * cardSpace);
		}
	}

	// ��ʾ��Ҵ������
	// �õ���ǰ��ҵĳ��������Լ��������
	QRect playCardRect = m_contextMap[player].playHandRect;
	Cards lastCards = m_contextMap[player].lastCards;
	if (!lastCards.isEmpty())
	{
		int playSpacing = 24;
		CardList lastCardList = lastCards.toCardList();
		CardList::ConstIterator itPlayed = lastCardList.constBegin();
		for (int i = 0; itPlayed != lastCardList.constEnd(); itPlayed++, i++)
		{
			CardPanel* panel = m_cardMap[*itPlayed];
			panel->setFrontSide(true);
			panel->raise();
			// ����������ƶ�����������
			if (m_contextMap[player].align == Horizontal)
			{
				int leftBase = playCardRect.left() + (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
				int top = playCardRect.top() + (playCardRect.height() - panel->height()) / 2;
				panel->move(leftBase + i * playSpacing, top);
			}
			else
			{
				int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
				int topBase = playCardRect.top();
				panel->move(left, topBase + i * playSpacing);
			}
			panel->show();
		}
	}
}

void GamePanel::showAnimation(AnimationType type, int bet)
{
	switch (type)
	{
	case GamePanel::SeqSingle:
	case GamePanel::SeqPair:
		m_animation->setFixedSize(250, 150);
		m_animation->move((width() - m_animation->width()) / 2, 200);
		m_animation->showSequence(AnimationWindow::Type(type));
		break;
	case GamePanel::Plane:
		m_animation->setFixedSize(800, 75);
		m_animation->move((width() - m_animation->width()) / 2, 200);
		m_animation->showPlane();
		break;
	case GamePanel::JokerBomb:
		m_animation->setFixedSize(250, 200);
		m_animation->move((width() - m_animation->width()) / 2, (height() - m_animation->height()) / 2 - 70);
		m_animation->showJokerBomb();
		break;
	case GamePanel::Bomb:
		m_animation->setFixedSize(180, 200);
		m_animation->move((width() - m_animation->width()) / 2, (height() - m_animation->height()) / 2 - 70);
		m_animation->showBomb();
		break;
	case GamePanel::Bet:
		m_animation->setFixedSize(160, 98);
		m_animation->move((width() - m_animation->width()) / 2, (height() - m_animation->height()) / 2 - 140);
		m_animation->showBetScore(bet);
		break;
	default:
		break;
	}
	m_animation->show();
}

void GamePanel::onDisposePlayHand(Player* player, Cards& cards)
{
	// �洢��Ҵ������
	auto it = m_contextMap.find(player);
	it->lastCards = cards;
	// 2. �������Ͳ�����Ϸ��Ч
	PlayHand hand(cards);
	PlayHand::HandType type = hand.getHandType();
	if (type == PlayHand::Hand_Plane || type == PlayHand::Hand_Plane_Two_Pair || type == PlayHand::Hand_Plane_Two_Single)
		showAnimation(Plane);
	else if (type == PlayHand::Hand_Seq_Pair)
		showAnimation(SeqPair);
	else if (type == PlayHand::Hand_Seq_Single)
		showAnimation(SeqSingle);
	else if (type == PlayHand::Hand_Bomb)
		showAnimation(Bomb);
	else if (type == PlayHand::Hand_Bomb_Jokers)
		showAnimation(JokerBomb);
	// �����Ҵ�����ǿ���(������)����ʾ��ʾ��Ϣ
	if (cards.isEmpty())
	{
		it->info->setPixmap(QPixmap(":/images/pass.png"));
		it->info->show();
		m_bgm->playPassMusic(BGMControl::RoleSex(player->getSex()));
	}
	else
	{
		if (m_gameCtl->getPendPlayer() == player || m_gameCtl->getPendPlayer() == nullptr)
			m_bgm->playCardMusic(cards, true, BGMControl::RoleSex(player->getSex()));
		else
			m_bgm->playCardMusic(cards, false, BGMControl::RoleSex(player->getSex()));
	}
	// 3. �������ʣ�����
	updatePlayerCards(player);
	// 4. ������ʾ����
	// �ж����ʣ����Ƶ�����
	if (player->getCards().cardCount() == 2)
		m_bgm->playLastMusic(BGMControl::Last2, BGMControl::RoleSex(player->getSex()));
	else if(player->getCards().cardCount() == 1)
		m_bgm->playLastMusic(BGMControl::Last1, BGMControl::RoleSex(player->getSex()));
}

void GamePanel::hidePlayerDropCards(Player* player)
{
	auto it = m_contextMap.find(player);
	if (it != m_contextMap.end())
	{
		if (it->lastCards.isEmpty())
			it->info->hide();
		else
		{
			// Cards --> Card
			CardList list = it->lastCards.toCardList();
			for (auto last = list.begin(); last != list.end(); last++)
				m_cardMap[*last]->hide();
		}
		it->lastCards.clear();
	}
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
	// ��ͼƬ
	QVector<QString> lordMan;
	QVector<QString> lordWoman;
	QVector<QString> farmerMan;
	QVector<QString> farmerWoman;

	lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png";
	lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png";
	farmerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png";
	farmerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png";

	// ����ͼƬ QImage
	QImage image;
	int random = QRandomGenerator::global()->bounded(2);
	if (sex == Player::Man && role == Player::Lord)
		image.load(lordMan[random]);
	else if (sex == Player::Man && role == Player::Farmer)
		image.load(farmerMan[random]);
	else if (sex == Player::Woman && role == Player::Lord)
		image.load(lordWoman[random]);
	else if (sex == Player::Woman && role == Player::Farmer)
		image.load(farmerWoman[random]);
	
	QPixmap pixmap;
	if (direct == Player::Left)
		pixmap = QPixmap::fromImage(image);
	else
		pixmap = QPixmap::fromImage(image.mirrored(true, false));

	return pixmap;
}

void GamePanel::showEndingScorePanel()
{
	bool isLord = m_gameCtl->getUserPlayer()->getRole() == Player::Lord ? true : false;
	bool isWin = m_gameCtl->getUserPlayer()->isWin();
	EndingPanel* panel = new EndingPanel(isLord, isWin, this);
	panel->show();
	panel->move((width() - panel->width()) / 2, -panel->height());
	panel->setPlayerScore(m_gameCtl->getLeftRobot()->getScore(),
							m_gameCtl->getRightRobot()->getScore(),
							m_gameCtl->getUserPlayer()->getScore());
	
	m_bgm->playEndingMusic(isWin);
	QPropertyAnimation* animation = new QPropertyAnimation(panel, "geometry", this);
	// ����������ʱ��
	animation->setDuration(1500);	// 1.5s
	// ���ô��ڵ���ʼλ�ú���ֹλ��
	animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
	animation->setEndValue(QRect((width() - panel->width()) / 2, (height() - panel->height()) / 2, panel->width(), panel->height()));
	// ���ô��ڵ��˶�����
	animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
	// ���Ŷ���Ч��
	animation->start();

	// �������ź�
	connect(panel, &EndingPanel::continueGame, this, [=]() {
		panel->close();
		panel->deleteLater();
		animation->deleteLater();
		btnGroup->selectPanel(ButtonGroup::Empty);
		gameStatusPrecess(GameControl::DispatchCard);
		m_bgm->startBGM(80);
	});
}

void GamePanel::initCountDown()
{
	m_countDown = new CountDown(this);
	m_countDown->move((width() - m_countDown->width()) / 2, (height() - m_countDown->height()) / 2 + 30);
	connect(m_countDown, &CountDown::notMuchTime, this, [=]() {
		// ������ʾ����
		m_bgm->playAssistMusic(BGMControl::Alert);
	});
	connect(m_countDown, &CountDown::timeOut, this, &GamePanel::onUserPass);

	UserPlayer* userPlayer = m_gameCtl->getUserPlayer();
	connect(userPlayer, &UserPlayer::startCountDown, this, [=]() {
		if (m_gameCtl->getPendPlayer() != userPlayer && m_gameCtl->getPendPlayer() != nullptr)
			m_countDown->showCountDown();
	});
}

void GamePanel::onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status)
{
	switch (status)
	{
	case GameControl::ThinkingForCallLord:
		if (player == m_gameCtl->getUserPlayer())
			btnGroup->selectPanel(ButtonGroup::CallLord, m_gameCtl->getPlayerMaxBet());
		break;
	case GameControl::ThinkingForPlayHand:
		// 1. ������һ�ִ������
		hidePlayerDropCards(player);
		if (player == m_gameCtl->getUserPlayer())
		{
			// ȡ��������ҵĶ���
			Player* pendPlayer = m_gameCtl->getPendPlayer();
			if (pendPlayer == m_gameCtl->getUserPlayer() || pendPlayer == nullptr)
				btnGroup->selectPanel(ButtonGroup::PlayCard);
			else
				btnGroup->selectPanel(ButtonGroup::PassOrPlay);
		}
		else
			btnGroup->selectPanel(ButtonGroup::Empty);
		break;
	case GameControl::winning:
		m_bgm->stopBGM();
		m_contextMap[m_gameCtl->getLeftRobot()].isFrontSide = true;
		m_contextMap[m_gameCtl->getRightRobot()].isFrontSide = true;
		updatePlayerCards(m_gameCtl->getLeftRobot());
		updatePlayerCards(m_gameCtl->getRightRobot());
		// ������ҵĵ÷�
		updatePlayerScore();
		m_gameCtl->setCurrentPlayer(player);
		showEndingScorePanel();
		break;
	default:
		break;
	}
}

void GamePanel::onGrabLordBet(Player* player, int bet, bool isFirst)
{
	// ��ʾ����������Ϣ��ʾ
	playerContext context = m_contextMap[player];
	if (bet == 0)
		context.info->setPixmap(QPixmap(":/images/buqinag.png"));
	else
	{
		if(isFirst)
			context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
		else
			context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
		// ��ʾ�е����ķ���
		showAnimation(Bet, bet);
	}
	context.info->show();

	// ���ŷ����ı�������
	m_bgm->playerRobLordMusic(bet, BGMControl::RoleSex(player->getSex()), isFirst);
}

void GamePanel::onCardSelected(Qt::MouseButton button)
{
	// 1. �ж��ǲ��ǳ���״̬
	if (m_gameStatus == GameControl::DispatchCard || m_gameStatus == GameControl::CallingLord)
		return;

	// 2. �жϷ����źŵ��Ƶ��������ǲ��ǵ�ǰ�û����
	CardPanel* panel = static_cast<CardPanel*>(sender());
	if (panel->getOwner() != m_gameCtl->getUserPlayer())
		return;
	// 3. ���浱ǰ��ѡ�еĴ��ڶ���
	m_curSelCard = panel;
	// 4. �жϲ�������������������Ҽ�
	if (button == Qt::LeftButton)
	{
		// �����˿��Ƶ�ѡ��״̬
		panel->setSelected(!panel->isSelected());
		// �����˿����ڴ����е���ʾ
		updatePlayerCards(panel->getOwner());
		// �����ɾ���˿��ƴ��ڶ���
		QSet<CardPanel*>::const_iterator it = m_selectCards.find(panel);
		if (it == m_selectCards.constEnd())
			m_selectCards.insert(panel);
		else
			m_selectCards.erase(it);
		m_bgm->playAssistMusic(BGMControl::SelectCard);
	}
	else if (button == Qt::RightButton)
	{
		// ���ó��ư�ť�Ĳۺ���
		onUserPlayHand();
	}
}

void GamePanel::onUserPlayHand()
{
	// �ж���Ϸ״̬�Ƿ��ǳ���״̬
	if (m_gameStatus != GameControl::PlayingHand)
		return;
	// �ж�����ǲ����û����
	if (m_gameCtl->getCurrentPlayer() != m_gameCtl->getUserPlayer())
		return;
	// �ж�Ҫ�������Ƿ��ǿ���
	if (m_selectCards.isEmpty())
		return;
	// �õ�Ҫ������Ƶ�����
	Cards cs;
	for (auto it = m_selectCards.begin(); it != m_selectCards.end(); it++)
	{
		Card card = (*it)->getCard();
		cs.add(card);
	}
	PlayHand hand(cs);
	PlayHand::HandType type = hand.getHandType();
	if (type == PlayHand::Hand_Unknown)
		return;
	// �жϵ�ǰ��ҵ����Ƿ��ܹ�ѹס�ϼҵ���
	if (m_gameCtl->getPendPlayer() != m_gameCtl->getUserPlayer())
	{
		Cards cards = m_gameCtl->getPendCards();
		if (!hand.canBeat(PlayHand(cards)))
			return;
	}
	m_countDown->stopCountDown();
	// ͨ����Ҷ������
	m_gameCtl->getUserPlayer()->playHand(cs);
	// �������
	m_selectCards.clear();
}

void GamePanel::onUserPass()
{
	m_countDown->stopCountDown();
	// �ж��ǲ����û����
	Player* currPlayer = m_gameCtl->getCurrentPlayer();
	Player* userPlayer = m_gameCtl->getUserPlayer();
	if (currPlayer != userPlayer)
		return;
	// �жϵ�ǰ�û�����ǲ�����һ�γ��Ƶ����(���Բ�����)
	Player* pendPlayer = m_gameCtl->getPendPlayer();
	if (pendPlayer == userPlayer || pendPlayer == nullptr)
		return;
	// ���һ���յ�Cards����
	Cards empty;
	userPlayer->playHand(empty);
	// ����û�ѡ�����(��ҿ���ѡ����һЩ�ƣ�����û�д��ȥ)
	for (auto it = m_selectCards.begin(); it != m_selectCards.end(); it++)
		(*it)->setSelected(false);
	m_selectCards.clear();
	// ������Ҵ������������
	updatePlayerCards(userPlayer);
}

void GamePanel::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	p.drawPixmap(rect(), m_bkImage);
}

void GamePanel::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint pt = event->pos();
		if (!m_cardsRect.contains(pt))
			m_curSelCard = nullptr;
		else
		{
			QList<CardPanel*> list = m_userCards.keys();
			for(int i = 0; i < list.size(); i ++)
			{
				CardPanel* panel = list.at(i);
				if (m_userCards[panel].contains(pt) && m_curSelCard != panel)
				{
					// ��������˿���
					panel->clicked();
					m_curSelCard = panel;
				}
			}
		}
	}
}
