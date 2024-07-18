#ifndef _FRAMELESSWINDOM_H_
#define _FRAMELESSWINDOM_H_

#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QApplication>
#include <windowsx.h>
#include <qt_windows.h>

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
	void mouseDoubleClickEvent(QMouseEvent* event) override;

#if 0
	void setAllWidgetsMouseTracking(QWidget* widget);
	bool eventFilter(QObject* watched, QEvent* event) override;

	// ���������״
	void setCursorShape(const QPoint& point);
#endif
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

private:
	bool LeftPressed = false;					// ����Ƿ���
	bool LeftPressedInTitle = false;			// ����Ƿ��ڱ���������
	QVector<QWidget*> TitleBarWidgets;			// ��¼�������еĿؼ�

	QPoint PressedPos;	// �������λ��
	QPoint WindowPos;	// �������Ͻǵ���������

	int padding = 5;
	Location hoverPos = Location::CENTER;
};

#endif // !_FRAMELESSWINDOM_H_

