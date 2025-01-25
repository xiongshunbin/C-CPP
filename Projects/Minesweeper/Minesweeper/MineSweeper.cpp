#include "MineSweeper.h"

const int N = 16;
const int M = 16;
const int blockSide = 32;
const double faceSide = (double)blockSide / 16 * 2 * 13;
const int borderW = (double)blockSide * 3 / 4;
const int borderH = (double)blockSide * 11 / 16;
const unsigned int maxTime = 999;
const int minMineNumber = -99;
const int mineNumber = 40;
const double numWidth = (double)blockSide * 11 / 16;
const double numHeight = (double)blockSide * 21 / 16;
const double numbgWidth = (double)blockSide * 41 / 16;
const double numbgHeight = (double)blockSide * 25 / 16;

Widget::Widget(QWidget* parent)
	: QWidget(parent), cnt(mineNumber), time(0)
{
	setObjectName("MineSweeper");
	data = new MineSweeperData(N, M, mineNumber);
	setFixedSize(blockSide * M + borderW * 2, blockSide * N + borderH * 3 + blockSide * 2);
	setStyleSheet("#MineSweeper{background-color:#c0c0c0;}");
	
	prevPos = QPoint(-1, -1);
	pressedPos = QPoint(-1, -1);

	svgRender = new QSvgRenderer(this);
	painter = new QPainter;

	loadImage();

	timer = new QTimer(this);

	connect(data, &MineSweeperData::sendWin, this, [&](bool isWin) {
		if (isWin)
		{
			isSuccess = true;
			timer->stop();
			update();
		}
	});

	// 计时器
	connect(timer, &QTimer::timeout, this, [&]() {
		time++;
		if (time > maxTime)  time = maxTime;
		update();
		});
}

Widget::~Widget()
{
	delete data;
	delete timer;
	delete svgRender;
	delete painter;

	delete pixmap_redMine;
	delete pixmap_mine;
	delete pixmap_mineWrong;
	delete pixmap_flag;
	delete pixmap_block;
	delete pixmap_numbg;
}

void Widget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	drawBorder();

	painter->begin(this);
	for (int i = 0; i < data->getN(); i++)
	{
		for (int j = 0; j < data->getM(); j++)
		{
			if (data->open[i][j])
			{
				if (data->isMine(i, j))
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), *pixmap_mine);
				else
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), pixmap_type[data->getNumber(i, j)]);
				if(i == data->overPos.first && j == data->overPos.second)
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), *pixmap_redMine);
			}
			else
			{
				if (data->flags[i][j])
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), *pixmap_flag);
				else if(data->pressed[i][j])
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), pixmap_type[0]);
				else
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), *pixmap_block);

				if(data->mineWrong[i][j])
					painter->drawPixmap(QPoint(borderW + j * blockSide, borderH * 2 + blockSide * 2 + i * blockSide), *pixmap_mineWrong);

			}
		}
	}

	if (isGameOver)
		painter->drawPixmap(QPoint(borderW + (double)blockSide * M / 2 - faceSide / 2,
			borderH + (double)blockSide * 2 * 3 / 32), pixmap_face[(int)MineSweeperData::faceType::lose]);
	else if(isSuccess)
		painter->drawPixmap(QPoint(borderW + (double)blockSide * M / 2 - faceSide / 2,
			borderH + (double)blockSide * 2 * 3 / 32), pixmap_face[(int)MineSweeperData::faceType::win]);
	else
		painter->drawPixmap(QPoint(borderW + (double)blockSide * M / 2 - faceSide / 2,
			borderH + (double)blockSide * 2 * 3 / 32), pixmap_face[(int)MineSweeperData::faceType::unpressed]);

	painter->end();

	drawCount(cnt);
	drawSecond(time);
}

// 绘制边框
void Widget::drawBorder()
{
	QPixmap pix;
	painter->begin(this);

	pix.load(":/res/corner_up_left_2x.png");
	painter->drawPixmap(0, 0, pix.scaled(borderW, borderH));

	pix.load(":/res/corner_up_right_2x.png");
	painter->drawPixmap(borderW + blockSide * M, 0, pix.scaled(borderW, borderH));

	pix.load(":/res/corner_bottom_left_2x.png");
	painter->drawPixmap(0, borderH * 2 + blockSide * N + blockSide * 2, pix.scaled(borderW, borderH));

	pix.load(":/res/corner_bottom_right_2x.png");
	painter->drawPixmap(borderW + blockSide * M, borderH * 2 + blockSide * N + blockSide * 2, pix.scaled(borderW, borderH));

	pix.load(":/res/border_hor_2x.png");
	painter->drawPixmap(borderW, 0, pix.scaled(blockSide * M, borderH));
	painter->drawPixmap(borderW, borderH + blockSide * 2, pix.scaled(blockSide * M, borderH));
	painter->drawPixmap(borderW, borderH * 2 + blockSide * 2 + blockSide * N, pix.scaled(blockSide * M, borderH));

	pix.load(":/res/border_vert_2x.png");
	painter->drawPixmap(0, borderH, pix.scaled(borderW, blockSide * 2));
	painter->drawPixmap(0, borderH * 2 + blockSide * 2, pix.scaled(borderW, blockSide * N));
	painter->drawPixmap(borderW + blockSide * M, borderH, pix.scaled(borderW, blockSide * 2));
	painter->drawPixmap(borderW + blockSide * M, borderH * 2 + blockSide * 2, pix.scaled(borderW, blockSide * N));

	pix.load(":/res/t_left_2x.png");
	painter->drawPixmap(0, borderH + blockSide * 2, pix.scaled(borderW, borderH));

	pix.load(":/res/t_right_2x.png");
	painter->drawPixmap(borderW + blockSide * M, borderH + blockSide * 2, pix.scaled(borderW, borderH));
	painter->end();
}

// 绘制计数器
void Widget::drawCount(int num)
{
	painter->begin(this);
	painter->drawPixmap(QPoint(borderW + (double)blockSide * 3 / 16,
		borderH + (double)blockSide * 3 / 16), *pixmap_numbg);

	int unit = num % 10;
	int ten = num / 10 % 10;
	int hundred = num / 100;

	if (num < minMineNumber)	num = minMineNumber;

	if (num < 0)
	{
		unit = -unit;
		ten = -ten;
		hundred = -1;
	}

	painter->drawPixmap(QPoint(borderW + (double)blockSide * 5 / 16,
		borderH + (double)blockSide * 5 / 16), pixmap_num[hundred + 1]);

	painter->drawPixmap(QPoint(borderW + (double)blockSide * 7 / 16 + numWidth,
		borderH + (double)blockSide * 5 / 16), pixmap_num[ten + 1]);

	painter->drawPixmap(QPoint(borderW + (double)blockSide * 9 / 16 + numWidth * 2,
		borderH + (double)blockSide * 5 / 16), pixmap_num[unit + 1]);
	painter->end();
}

// 绘制计时器
void Widget::drawSecond(int num)
{
	painter->begin(this);
	painter->drawPixmap(QPoint(borderW + blockSide * M - (double)blockSide * 3 / 16 - numbgWidth,
		borderH + (double)blockSide * 3 / 16), *pixmap_numbg);

	int unit = num % 10;
	int ten = num / 10 % 10;
	int hundred = num / 100;

	painter->drawPixmap(QPoint(borderW + blockSide * M - (double)blockSide * 1 / 16 - numbgWidth,
		borderH + (double)blockSide * 5 / 16), pixmap_num[hundred + 1]);

	painter->drawPixmap(QPoint(borderW + blockSide * M + (double)blockSide * 1 / 16 - numbgWidth + numWidth,
		borderH + (double)blockSide * 5 / 16), pixmap_num[ten + 1]);

	painter->drawPixmap(QPoint(borderW + blockSide * M + (double)blockSide * 3 / 16 - numbgWidth + numWidth * 2,
		borderH + (double)blockSide * 5 / 16), pixmap_num[unit + 1]);
	painter->end();
}

// 处理鼠标点击事件
void Widget::mouseReleaseEvent(QMouseEvent* event)
{
	int x = (event->y() - borderH * 2 - blockSide * 2) / blockSide;
	int y = (event->x() - borderW) / blockSide;

	if (event->button() == Qt::LeftButton)
		setData(true, x, y);
	else
		setData(false, x, y);
}

void Widget::mousePressEvent(QMouseEvent* event)
{
	if (isGameOver || isSuccess) return;

	int x = (event->y() - borderH * 2 - blockSide * 2) / blockSide;
	int y = (event->x() - borderW) / blockSide;

	if (event->button() == Qt::LeftButton && data->inArea(x, y))
	{
		if (!data->open[x][y])	data->pressed[x][y] = true;
		else					data->whenPressed(true, x, y);
		prevPos = QPoint(x, y);
		pressedPos = QPoint(x, y);
	}
	update();
}

void Widget::mouseMoveEvent(QMouseEvent* event)
{
	if (isGameOver || isSuccess) return;

	if (event->buttons() & Qt::LeftButton) {
		int x = (event->y() - borderH * 2 - blockSide * 2) / blockSide;
		int y = (event->x() - borderW) / blockSide;
		
		if (x != prevPos.x() || y != prevPos.y())
		{
			data->whenPressed(false, pressedPos.x(), pressedPos.y());
			data->whenPressed(false, prevPos.x(), prevPos.y());

			data->pressed[prevPos.x()][prevPos.y()] = false;
			if (data->inArea(x, y))
			{
				prevPos = QPoint(x, y);
				data->pressed[x][y] = true;
				if(data->open[x][y])
					data->whenPressed(true, x, y);
			}
			update();
		}
	}
}

// 数据更新
void Widget::setData(bool isLeftClicked, int x, int y)
{
	if (isGameOver || isSuccess) return;

	if (data->inArea(x, y))
	{
		if (!isStart)
		{
			if (data->isMine(x, y))	data->stepSquare(x, y);
			isStart = true;
			timer->start(1000);
		}

		if (isLeftClicked)
		{
			if (data->isMine(x, y) && !data->flags[x][y])
			{
				isGameOver = true;
				data->open[x][y] = true;
				data->overPos = { x, y };
				gameOver();
			}
			else
				data->reveal(x, y);
		}
		else
		{
			if (!data->open[x][y])
			{
				data->flags[x][y] ? (cnt++, ++(data->operate)) : (cnt--, (data->operate)--);
				data->flags[x][y] = !data->flags[x][y];
				if (!cnt && data->checkSuccess())
				{
					isSuccess = true;
					timer->stop();
				}
			}
		}
		data->whenPressed(false, pressedPos.x(), pressedPos.y());
		data->whenPressed(false, prevPos.x(), prevPos.y());
	}
	update();
}

// 导入并渲染svg图片，加快图片绘制速度
void Widget::loadImage()
{
	pixmap_redMine = new QPixmap(blockSide, blockSide);
	loadSvg(pixmap_redMine, MineSweeperData::redMineImageURL);

	pixmap_mine = new QPixmap(blockSide, blockSide);
	loadSvg(pixmap_mine, MineSweeperData::mineImageURL);

	pixmap_mineWrong = new QPixmap(blockSide, blockSide);
	loadSvg(pixmap_mineWrong, MineSweeperData::mineWrongImageURL);

	pixmap_flag = new QPixmap(blockSide, blockSide);
	loadSvg(pixmap_flag, MineSweeperData::flagImageURL);

	pixmap_block = new QPixmap(blockSide, blockSide);
	loadSvg(pixmap_block, MineSweeperData::blockImageURL);

	pixmap_numbg = new QPixmap(numbgWidth, numbgHeight);
	loadSvg(pixmap_numbg, MineSweeperData::nums_backgroundURL);

	for (int i = 0; i < 9; i++)
	{
		QPixmap pix(blockSide, blockSide);
		loadSvg(&pix, MineSweeperData::typeImageURL(i));
		pixmap_type.push_back(pix);
	}

	for (int i = -1; i < 10; i++)
	{
		QPixmap pix(numWidth, numHeight);
		loadSvg(&pix, MineSweeperData::numberImageURL(i));
		pixmap_num.push_back(pix);
	}

	for (int i = 0; i < 4; i++)
	{
		QPixmap pix(faceSide, faceSide);
		loadSvg(&pix, MineSweeperData::faceImageURL(MineSweeperData::faceType(i)));
		pixmap_face.push_back(pix);
	}
}

// 加载svg图片并绘制到对应的pixmap上面
void Widget::loadSvg(QPixmap* pixmap, QString fileName)
{
	pixmap->fill(Qt::transparent);
	painter->begin(pixmap);
	svgRender->load(fileName);
	svgRender->render(painter);
	painter->end();
}

// 游戏结束处理
void Widget::gameOver()
{
	for (int i = 0; i < data->getN(); i++)
	{
		for (int j = 0; j < data->getM(); j++)
		{
			if (data->isMine(i, j) && !data->flags[i][j] && !data->open[i][j])
					data->open[i][j] = true;
			if (data->flags[i][j] && !data->isMine(i, j))
				data->mineWrong[i][j] = true;
		}
	}
	timer->stop();
}