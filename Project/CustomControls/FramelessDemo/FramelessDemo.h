#ifndef _FRAMELESSDEMO_H_
#define _FRAMELESSDEMO_H_

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFontDatabase>
#include  <QEvent>

#include "FramelessWindow.h"

class FramelessDemo : public FramelessWindow
{
	Q_OBJECT

public:
	FramelessDemo(QWidget* parent = nullptr);
	~FramelessDemo();

private:
	void initForm();
	void initTitleBar();

protected:
	void changeEvent(QEvent* event) override;

private:
	QWidget* TitleBar;      // ±ÍÃ‚¿∏
	QLabel* LblIcon;        // Õº±Í
	QLabel* LblTitle;
	QPushButton* BtnMin;
	QPushButton* BtnMax;
	QPushButton* BtnClose;

	QWidget* MainWidget;
};


#endif // !_FRAMELESSDEMO_H_