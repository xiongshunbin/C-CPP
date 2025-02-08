#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QTimer>

class AnimationWindow : public QWidget
{
	Q_OBJECT
public:
	enum Type{SeqSingle, SeqPair};
	explicit AnimationWindow(QWidget* parent = nullptr);

	// ��ʾ��ע����
	void showBetScore(int bet);

	// ��ʾ˳�Ӻ�����
	void showSequence(Type type);

	// ��ʾ��ը
	void showJokerBomb();

	// ��ʾը��
	void showBomb();

	// ��ʾ�ɻ�
	void showPlane();


protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPixmap m_image;
	int m_index = 0;
	int m_x = 0;
};

