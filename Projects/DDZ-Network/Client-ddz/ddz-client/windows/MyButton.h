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
	// 鼠标按下
	void mousePressEvent(QMouseEvent* event) override;
	// 鼠标释放
	void mouseReleaseEvent(QMouseEvent* event) override;
	// 鼠标进入
	void enterEvent(QEvent* event) override;
	// 鼠标离开
	void leaveEvent(QEvent* event) override;
	// 绘图
	void paintEvent(QPaintEvent* event) override;

private:
	QString m_normal;
	QString m_hover;
	QString m_pressed;
	QPixmap m_pixmap;
};

