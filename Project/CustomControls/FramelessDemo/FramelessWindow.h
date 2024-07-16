#ifndef _FRAMELESSWINDOM_H_
#define _FRAMELESSWINDOM_H_

#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QApplication>

class FramelessWindow : public QWidget
{
	Q_OBJECT
public:
	enum class Location : short 
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		CENTER
	};
	explicit FramelessWindow(QWidget* parent = nullptr);

protected:
	void setTitleBarWidgets(QVector<QWidget*> widgets);
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void setAllWidgetsMouseTracking(QWidget* widget);
	bool eventFilter(QObject* watched, QEvent* event) override;

	// 设置鼠标形状
	void setCursorShape(const QPoint& point);

private:
	bool LeftPressed = false;					// 左键是否按下
	bool LeftPressedInTitle = false;			// 左键是否在标题栏按下
	QVector<QWidget*> TitleBarWidgets;			// 记录标题栏中的控件

	QPoint PressedPos;	// 鼠标点击的位置
	QPoint WindowPos;	// 窗口左上角的世界坐标

	int padding = 5;
	Location hoverPos = Location::CENTER;
};

#endif // !_FRAMELESSWINDOM_H_

