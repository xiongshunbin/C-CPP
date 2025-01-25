#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QFont>
#include <QString>
#include <QVector>

class ScorePanel : public QWidget
{
	Q_OBJECT
public:
	enum FontColor{Black, White, Red, Blue, Green};
	explicit ScorePanel(QWidget* parent = nullptr);
	~ScorePanel();

	void panelInit();
	void setSameStyle(QLabel *label, QString text = "");

	void setScores(int left, int right, int user);

	// ���������С
	void setMyFontSize(int point);
	// ������ɫ
	void setMyFontColor(FontColor color);

private:
	QLabel* myTitle;
	QLabel* myScore;
	QLabel* score1;

	QLabel* leftTitle;
	QLabel* leftScore;
	QLabel* score2;

	QLabel* rightTitle;
	QLabel* rightScore;
	QLabel* score3;

	QVector<QLabel*> m_list;
};

