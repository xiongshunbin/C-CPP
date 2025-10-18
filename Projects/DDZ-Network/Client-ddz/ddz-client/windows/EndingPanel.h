#pragma once

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include "ScorePanel.h"

class EndingPanel : public QWidget
{
	Q_OBJECT
public:
	EndingPanel(bool isLord, bool isWin, QWidget* parent = nullptr);
	void setPlayerScore(int left, int right, int me);

signals:
	void continueGame();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPixmap m_bg;
	QLabel* m_title;
	ScorePanel* m_score;
	QPushButton* m_continue;
};

