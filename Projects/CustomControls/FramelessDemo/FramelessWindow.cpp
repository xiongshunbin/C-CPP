#include "FramelessWindow.h"
#include <QDebug>
#include <QDesktopWidget>

FramelessWindow::FramelessWindow(QWidget* parent) : QWidget(parent)
{
	// 去掉标题栏
	//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	// 防止窗口的大小未及时恢复到正常值
	setAttribute(Qt::WA_TranslucentBackground);

	//this->installEventFilter(this);
	setAttribute(Qt::WA_Hover);
}

void FramelessWindow::setTitleBarWidgets(QVector<QWidget*> widgets)
{
	TitleBarWidgets = widgets;
}

void FramelessWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
		return;

	LeftPressed = true;

	WindowPos = this->pos();			// 记录当前窗体左上角的世界坐标
	PressedPos = event->globalPos();	// 记录鼠标按下的位置

	QWidget* PressedWidget = QApplication::widgetAt(event->globalPos());
	if (PressedWidget)
	{
		foreach(QWidget * widget, TitleBarWidgets)
		{
			if (PressedWidget == widget)
			{
				LeftPressedInTitle = true;
				break;
			}
		}
	}
}

void FramelessWindow::mouseMoveEvent(QMouseEvent* event)
{
	QPoint globalPos = event->globalPos();

	// 左键未按下
	if (!LeftPressed)
	{
		/*
		if (this->windowState().testFlag(Qt::WindowNoState))
		{
			setCursorShape(globalPos);
		}
		*/
		return;
	}
	// 左键按下
	if (hoverPos != Location::CENTER)
	{
		// 在边界处按下
		QRect rect = this->rect();
		QPoint topLeft = mapToGlobal(rect.topLeft());
		QPoint bottomRight = mapToGlobal(rect.bottomRight());

		QRect rMove(topLeft, bottomRight);

		switch (hoverPos)
		{
		case Location::TOP:
			// 防止窗口被推走
			if(bottomRight.y() - globalPos.y() > this->minimumHeight())
				rMove.setY(globalPos.y());
			break;
		case Location::BOTTOM:
			rMove.setHeight(globalPos.y() - topLeft.y());
			break;
		case Location::LEFT:
			// 防止窗口被推走
			if(bottomRight.x() - globalPos.x() > this->minimumWidth())
				rMove.setX(globalPos.x());
			break;
		case Location::RIGHT:
			rMove.setWidth(globalPos.x() - topLeft.x());
			break;
		case Location::TOP_LEFT:
			if (bottomRight.y() - globalPos.y() > this->minimumHeight())
				rMove.setY(globalPos.y());
			if (bottomRight.x() - globalPos.x() > this->minimumWidth())
				rMove.setX(globalPos.x());
			break;
		case Location::TOP_RIGHT:
			if (bottomRight.y() - globalPos.y() > this->minimumHeight())
				rMove.setY(globalPos.y());
			rMove.setWidth(globalPos.x() - topLeft.x());
			break;
		case Location::BOTTOM_LEFT:
			rMove.setHeight(globalPos.y() - topLeft.y());
			if (bottomRight.x() - globalPos.x() > this->minimumWidth())
				rMove.setX(globalPos.x());
			break;
		case Location::BOTTOM_RIGHT:
			rMove.setHeight(globalPos.y() - topLeft.y());
			rMove.setWidth(globalPos.x() - topLeft.x());
			break;
		case Location::CENTER:
			break;
		default:
			break;
		}

		this->setGeometry(rMove);
	}
	else
	{
		// 在标题栏按下
		if (LeftPressedInTitle)
		{
			if (this->isMaximized())
			{
				// 窗口最大化时，鼠标拖动标题栏
				// a.窗口恢复
				double width_ratio = (double)event->pos().x() / width();
				qDebug() << width();
				this->setWindowState(Qt::WindowNoState);
				// b.鼠标相对窗口的位置不变
				qDebug() << rect().width();
				int offset = width() * width_ratio;

				WindowPos.setX(event->globalPos().x() - offset);
				WindowPos.setY(0);
			}
			else
				this->move(WindowPos + event->globalPos() - PressedPos);
		}
	}
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent* event)
{
	LeftPressed = false;
	LeftPressedInTitle = false;
	//this->setCursor(QCursor(Qt::ArrowCursor));
}

// 双击标题栏
void FramelessWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	QWidget* doubleClickedWidget = QApplication::widgetAt(event->globalPos());
	if (doubleClickedWidget)
	{
		bool inTitle = false;
		foreach(QWidget * widget, TitleBarWidgets)
		{
			if (doubleClickedWidget == widget)
			{
				inTitle = true;
				break;
			}
		}

		if (inTitle)
		{
			this->setWindowState(isMaximized() ? Qt::WindowNoState : Qt::WindowMaximized);
		}
	}
}

# if 0
void FramelessWindow::setAllWidgetsMouseTracking(QWidget* widget)
{
	widget->setMouseTracking(true);

	QObjectList list = widget->children();
	foreach(QObject * obj, list)
	{
		qDebug() << obj->metaObject()->className();
		QWidget* w = (QWidget*)obj;
		if (obj->metaObject()->className() == QStringLiteral("QWidget"))
			setAllWidgetsMouseTracking(w);
		else if (obj->metaObject()->className() == QStringLiteral("QVBoxLayout")
			|| obj->metaObject()->className() == QStringLiteral("QHBoxLayout"))
			continue;
		w->setMouseTracking(true);
	}
}

// 当前窗口绘制时，给窗口中的所有控件设置鼠标追踪，这样当鼠标移动到窗口边界时，可以追踪到鼠标并设置光标缩放形状
bool FramelessWindow::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Paint)
	{
		static bool init = false;
		if (!init)
		{
			init = true;
			setAllWidgetsMouseTracking(this);
		}
	}

	// 父类的实现就是return false，表示让事件接着传递。也就是传递给对应的控件
	return QWidget::eventFilter(watched, event);
}


void FramelessWindow::setCursorShape(const QPoint& point)
{
	QRect rect = this->rect();

	QPoint topLeft = mapToGlobal(rect.topLeft());
	QPoint bottomRight = mapToGlobal(rect.bottomRight());

	int x = point.x(), y = point.y();

	if (x >= topLeft.x() && x <= topLeft.x() + padding && y >= topLeft.y() && y <= topLeft.y() + padding)
	{
		// 左上角
		hoverPos = Location::TOP_LEFT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x() && y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// 右下角
		hoverPos = Location::BOTTOM_RIGHT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= topLeft.x() && x <= topLeft.x() + padding && y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// 左下角
		hoverPos = Location::BOTTOM_LEFT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x() && y >= topLeft.y() && y <= topLeft.y() + padding)
	{
		// 右上角
		hoverPos = Location::TOP_RIGHT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= topLeft.x() && x <= topLeft.x() + padding)
	{
		// 左边
		hoverPos = Location::LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x())
	{
		// 右边
		hoverPos = Location::RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= topLeft.y() && y <= topLeft.y() + padding)
	{
		// 上边
		hoverPos = Location::TOP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// 下边
		hoverPos = Location::BOTTOM;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else
	{
		hoverPos = Location::CENTER;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}
#endif

bool FramelessWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	int m_nBorderWidth = 5;

	MSG* param = static_cast<MSG*>(message);

	switch (param->message)
	{
	case WM_NCHITTEST:
	{
		int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
		int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

		// 当窗口最大化时，不允许鼠标拖动缩放，当鼠标拖动标题栏时，窗口恢复默认大小
		if(windowState().testFlag(Qt::WindowMaximized))
			return QWidget::nativeEvent(eventType, message, result);

		if (nX > m_nBorderWidth && nX < this->width() - m_nBorderWidth &&
			nY > m_nBorderWidth && nY < this->height() - m_nBorderWidth)
		{
			if (childAt(nX, nY) != nullptr)
				return QWidget::nativeEvent(eventType, message, result);
		}

		if (nX > 0 && nX < m_nBorderWidth)
			*result = HTLEFT;

		if (nX > this->width() - m_nBorderWidth && nX < this->width())
			*result = HTRIGHT;

		if (nY > 0 && nY < m_nBorderWidth)
			*result = HTTOP;

		if (nY > this->height() - m_nBorderWidth && nY < this->height())
			*result = HTBOTTOM;

		if (nX > 0 && nX < m_nBorderWidth && nY > 0 && nY < m_nBorderWidth)
			*result = HTTOPLEFT;

		if (nX > this->width() - m_nBorderWidth && nX < this->width() &&
			nY > 0 && nY < m_nBorderWidth)
			*result = HTTOPRIGHT;

		if (nX > 0 && nX < m_nBorderWidth &&
			nY > this->height() - m_nBorderWidth && nY < this->height())
			*result = HTBOTTOMLEFT;

		if (nX > this->width() - m_nBorderWidth && nX < this->width() &&
			nY > this->height() - m_nBorderWidth && nY < this->height())
			*result = HTBOTTOMRIGHT;

		return true;
	}
	}
	return false;
}