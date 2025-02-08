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

	// ���û�ȡͼƬ����
	void setImage(const QPixmap& front, const QPixmap& back);
	QPixmap getImage();

	// �˿�����ʾ��һ��
	void setFrontSide(bool flag);
	bool isFrontSide();

	// ��¼�˿��ƴ����Ƿ�ѡ��
	void setSelected(bool flag);
	bool isSelected();

	// �˿��ƵĻ�ɫ�Լ�����
	void setCard(const Card &card);
	Card getCard();

	// �˿��Ƶ�������
	void setOwner(Player* player);
	Player* getOwner();

	// ģ���˿��Ƶĵ���¼�
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

