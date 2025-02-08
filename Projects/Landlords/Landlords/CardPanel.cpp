#include "CardPanel.h"

CardPanel::CardPanel(QWidget* parent) : QWidget(parent)
{         
}

void CardPanel::setImage(const QPixmap& front, const QPixmap& back)
{
	m_front = front;
	m_back = back;

	setFixedSize(m_front.size());

	update();
}

QPixmap CardPanel::getImage()
{
	return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
	m_isFront = flag;
}

bool CardPanel::isFrontSide()
{
	return m_isFront;
}

void CardPanel::setSelected(bool flag)
{
	m_isSelected = flag;
}

bool CardPanel::isSelected()
{
	return m_isSelected;
}

void CardPanel::setCard(const Card& card)
{
	m_card = card;
}

Card CardPanel::getCard()
{
	return m_card;
}

void CardPanel::setOwner(Player* player)
{
	m_owner = player;
}

Player* CardPanel::getOwner()
{
	return m_owner;
}

void CardPanel::clicked()
{
	emit cardSelected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	if (m_isFront)
		p.drawPixmap(rect(), m_front);	// rect()��ȡ��ǰ���ڵĳߴ���Ϣ
	else
		p.drawPixmap(rect(), m_back);

}

void CardPanel::mousePressEvent(QMouseEvent* event)
{
	emit cardSelected(event->button());
}
