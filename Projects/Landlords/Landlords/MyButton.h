#pragma once

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

class MyButton : public QPushButton
{
	Q_OBJECT
public:
	explicit MyButton(QWidget* parent = nullptr);

	void setImage(QString normal, QString hover, QString pressed);;

protected:
	// ��갴��
	void mousePressEvent(QMouseEvent* event) override;
	// ����ͷ�
	void mouseReleaseEvent(QMouseEvent* event) override;
	// ������
	void enterEvent(QEvent* event) override;
	// ����뿪
	void leaveEvent(QEvent* event) override;
	// ��ͼ
	void paintEvent(QPaintEvent* event) override;

private:
	QString m_normal;
	QString m_hover;
	QString m_pressed;
	QPixmap m_pixmap;
};

