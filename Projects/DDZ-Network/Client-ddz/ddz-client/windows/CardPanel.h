#pragma once
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "Card.h"
#include "Player.h"

class CardPanel : public QWidget
{
	Q_OBJECT
public:
	explicit CardPanel(QWidget* parent = nullptr);

	// 设置获取图片函数
	void setImage(const QPixmap& front, const QPixmap& back);
	QPixmap getImage();

	// 扑克牌显示哪一面
	void setFrontSide(bool flag);
	bool isFrontSide();

	// 记录扑克牌窗口是否被选中
	void setSelected(bool flag);
	bool isSelected();

	// 扑克牌的花色以及点数
	void setCard(const Card &card);
	Card getCard();

	// 扑克牌的所有者
	void setOwner(Player* player);
	Player* getOwner();

	// 模拟扑克牌的点击事件
	void clicked();



protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void cardSelected(Qt::MouseButton button);

private:
	QPixmap m_front;
	QPixmap m_back;
	bool m_isFront = true;
	bool m_isSelected = false;
	Card m_card;
	Player* m_owner = nullptr;
};

