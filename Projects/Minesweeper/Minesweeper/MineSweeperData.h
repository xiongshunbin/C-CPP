#pragma once

#include <QString>
#include <QObject>
#include <vector>
#include <ctime>
#include <QDebug>
#include <QPair>

using std::vector;

class MineSweeperData : public QObject
{
	Q_OBJECT
private:
	int N, M;
	vector<vector<bool>> mines;
	vector<vector<int>> numbers;
	void generateMines(int mineNumber);
	void swap(int x1, int y1, int x2, int y2);
	void calculateNumbers();

public:
	enum class faceType { unpressed = 0, pressed = 1, lose = 2, win = 3 };
	int operate;
	vector<vector<bool>> open;
	vector<vector<bool>> flags;
	vector<vector<bool>> mineWrong;
	vector<vector<bool>> pressed;

	static const QString blockImageURL;
	static const QString flagImageURL;
	static const QString mineImageURL;
	static const QString redMineImageURL;
	static const QString mineWrongImageURL;
	static const QString nums_backgroundURL;
	static const QString typeImageURL(int num);
	static const QString numberImageURL(int num);
	static const QString faceImageURL(faceType type);
	QPair<int, int> overPos{-1, -1};
	MineSweeperData(int N, int M, int mineNumber, QObject *parent = nullptr);
	int getN() { return N; }
	int getM() { return M; }
	bool isMine(int x, int y) { return mines[x][y]; }
	bool inArea(int x, int y) { return x >= 0 && x < N&& y >= 0 && y < M; }
	int getNumber(int x, int y) { return numbers[x][y]; }
	void reveal(int x, int y);
	void stepSquare(int x, int y);
	bool checkSuccess();
	void whenPressed(bool isPressed, int x, int y);


signals:
	void sendWin(bool isWin);
};

