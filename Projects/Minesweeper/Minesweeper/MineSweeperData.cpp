#include "MineSweeperData.h"

const QString MineSweeperData::blockImageURL = ":/res/closed.svg";
const QString MineSweeperData::flagImageURL = ":/res/flag.svg";
const QString MineSweeperData::mineImageURL = ":/res/mine.svg";
const QString MineSweeperData::redMineImageURL = ":/res/mine_red.svg";
const QString MineSweeperData::nums_backgroundURL = ":/res/nums_background.svg";
const QString MineSweeperData::mineWrongImageURL = ":/res/mine_wrong.svg";


// 利用Fisher-Yates-Knuth洗牌算法来随机分布雷
void MineSweeperData::generateMines(int mineNumber)
{
	for (int i = 0; i < mineNumber; i++)
	{
		int x = i / M;
		int y = i % M;
		mines[x][y] = true;
	}

	for (int i = N * M - 1; i >= 0; i--)
	{
		int iX = i / M;
		int iY = i % M;

		int randNumber = rand() % (i + 1);

		int randX = randNumber / M;
		int randY = randNumber % M;

		swap(iX, iY, randX, randY);
	}
}

void MineSweeperData::swap(int x1, int y1, int x2, int y2)
{
	bool t = mines[x1][y1];
	mines[x1][y1] = mines[x2][y2];
	mines[x2][y2] = t;

}

void MineSweeperData::calculateNumbers()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (mines[i][j])
				numbers[i][j] = -1;

			numbers[i][j] = 0;

			for (int ii = i - 1; ii <= i + 1; ii++)
				for (int jj = j - 1; jj <= j + 1; jj++)
					if (inArea(ii, jj) && mines[ii][jj])
						numbers[i][j] ++;
		}
	}
}

const QString MineSweeperData::typeImageURL(int num)
{
	return QString(":/res/type%1.svg").arg(num);
}

const QString MineSweeperData::numberImageURL(int num)
{
	return QString(":/res/d%1.svg").arg(num);
}

const QString MineSweeperData::faceImageURL(faceType type)
{
	QString fileName;
	switch (type)
	{
	case faceType::unpressed:
		fileName = ":/res/face_unpressed.svg";
		break;
	case faceType::pressed:
		fileName = ":/res/face_pressed.svg";
		break;
	case faceType::lose:
		fileName = ":/res/face_lose.svg";
		break;
	case faceType::win:
		fileName = ":/res/face_win.svg";
	}
	return fileName;
}

MineSweeperData::MineSweeperData(int N, int M, int mineNumber, QObject* parent)
	:QObject(parent)
{
	srand(time(nullptr));
	this->N = N;
	this->M = M;

	mines.resize(N, vector<bool>(M, false));
	open.resize(N, vector<bool>(M, false));
	flags.resize(N, vector<bool>(M, false));
	numbers.resize(N, vector<int>(M, 0));
	mineWrong.resize(N, vector<bool>(M, false));
	pressed.resize(N, vector<bool>(M, false));

	operate = N * M;

	generateMines(mineNumber);

	calculateNumbers();
}


// 利用floodfill算法来展开空白区域
void MineSweeperData::reveal(int x, int y)
{
	if (inArea(x, y) && !isMine(x, y) && !flags[x][y] && !open[x][y])
	{
		open[x][y] = true;
		operate--;

		if (operate == 0)
			emit sendWin(checkSuccess());

		if (numbers[x][y] > 0)
			return;

		for (int i = x - 1; i <= x + 1; i++)
			for (int j = y - 1; j <= y + 1; j++)
				if (inArea(i, j) && !open[i][j] && !mines[i][j])
					reveal(i, j);
	}
}

// 避免开局是雷的情况
void MineSweeperData::stepSquare(int x, int y)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (!isMine(i, j))
				swap(i, j, x, y);

	calculateNumbers();
}

// 检查游戏是否胜利
bool MineSweeperData::checkSuccess()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (!flags[i][j] && !open[i][j])		return false;
			if (flags[i][j] && !isMine(i, j))		return false;
		}
	}
	return true;
}

void MineSweeperData::whenPressed(bool isPressed,int x, int y)
{
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
			if (inArea(i, j) && !flags[i][j] && !open[i][j])
				pressed[i][j] = isPressed;
}
