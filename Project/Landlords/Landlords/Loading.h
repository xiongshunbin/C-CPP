#pragma once

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "GamePanel.h"

class Loading : public QWidget
{
	Q_OBJECT
public:
	explicit Loading(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPixmap m_bg;
	QPixmap m_progress;
	int m_dist = 15;
};

