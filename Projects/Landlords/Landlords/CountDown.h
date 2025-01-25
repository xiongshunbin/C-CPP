#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class CountDown : public QWidget
{
	Q_OBJECT
public:
	explicit CountDown(QWidget* parent = nullptr);

	void showCountDown();
	void stopCountDown();

signals:
	void notMuchTime();
	void timeOut();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPixmap m_clock;
	QPixmap m_number;
	QTimer* m_timer;
	int m_count;
};

