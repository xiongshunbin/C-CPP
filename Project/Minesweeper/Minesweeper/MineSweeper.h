#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>
#include <QSvgRenderer>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include "MineSweeperData.h"

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget* parent = nullptr);
	~Widget();

	void paintEvent(QPaintEvent* event);

	void drawBorder();

	void drawCount(int num);

	void drawSecond(int num);

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void setData(bool isLeftClicked, int x, int y);

	void loadImage();

	void loadSvg(QPixmap* pixmap, QString fileName);

	void gameOver();

private:
	int cnt;
	unsigned int time;
	MineSweeperData* data;
	bool isStart = false;
	bool isGameOver = false;
	bool isSuccess = false;
	QTimer* timer;

	QSvgRenderer* svgRender = nullptr;
	QPainter *painter = nullptr;

	vector<QPixmap> pixmap_border;
	vector<QPixmap> pixmap_type;
	vector<QPixmap> pixmap_num;
	vector<QPixmap> pixmap_face;
	QPixmap* pixmap_redMine = nullptr;
	QPixmap* pixmap_mine = nullptr;
	QPixmap* pixmap_mineWrong = nullptr;
	QPixmap* pixmap_flag = nullptr;
	QPixmap* pixmap_block = nullptr;
	QPixmap* pixmap_numbg = nullptr;

	QPoint prevPos;
	QPoint pressedPos;
};
#endif // MINESWEEPER_H
