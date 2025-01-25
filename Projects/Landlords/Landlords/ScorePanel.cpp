#include "ScorePanel.h"

const QString MYCOLOR[] = { "black", "white", "red", "blue", "green" };

ScorePanel::ScorePanel(QWidget* parent) : QWidget(parent)
{
	panelInit();

	m_list << myTitle << leftTitle << rightTitle
		   << myScore << leftScore << rightScore
		   << score1 << score2 << score3;
}

ScorePanel::~ScorePanel()
{
}

void ScorePanel::panelInit()
{
	myTitle = new QLabel(this);
	setSameStyle(myTitle, u8"ÎÒ");

	myScore = new QLabel(this);
	setSameStyle(myScore);

	score1 = new QLabel(this);
	setSameStyle(score1, u8"·Ö");

	leftTitle = new QLabel(this);
	setSameStyle(leftTitle, u8"×ó²à»úÆ÷ÈË");

	leftScore = new QLabel(this);
	setSameStyle(leftScore);

	score2 = new QLabel(this);
	setSameStyle(score2, u8"·Ö");

	rightTitle = new QLabel(this);
	setSameStyle(rightTitle, u8"ÓÒ²à»úÆ÷ÈË");

	rightScore = new QLabel(this);
	setSameStyle(rightScore);

	score3 = new QLabel(this);
	setSameStyle(score3, u8"·Ö");

	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(myTitle, 0, 0);
	layout->addWidget(myScore, 0, 1);
	layout->addWidget(score1, 0, 2);

	layout->addWidget(leftTitle, 1, 0);
	layout->addWidget(leftScore, 1, 1);
	layout->addWidget(score2, 1, 2);

	layout->addWidget(rightTitle, 2, 0);
	layout->addWidget(rightScore, 2, 1);
	layout->addWidget(score3, 2, 2);
}

void ScorePanel::setSameStyle(QLabel* label, QString text)
{
	label->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 12, QFont::Bold));
	label->setAlignment(Qt::AlignCenter);
	label->setText(text);
	label->setStyleSheet("color: #ffffff;");
}

void ScorePanel::setScores(int left, int right, int user)
{
	leftScore->setText(QString::number(left));
	rightScore->setText(QString::number(right));
	myScore->setText(QString::number(user));
}

void ScorePanel::setMyFontSize(int point)
{
	QFont font(u8"Î¢ÈíÑÅºÚ", point, QFont::Bold);
	for (int i = 0; i < m_list.size(); i++)
		m_list[i]->setFont(font);
}

void ScorePanel::setMyFontColor(FontColor color)
{
	QString style = QString("QLabel{color: %1}").arg(MYCOLOR[color]);
	for (int i = 0; i < m_list.size(); i++)
		m_list[i]->setStyleSheet(style);
}
