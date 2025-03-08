#include "FramelessWindow.h"
#include <QDebug>
#include <QDesktopWidget>

FramelessWindow::FramelessWindow(QWidget* parent) : QWidget(parent)
{
	// ȥ��������
	//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	// ��ֹ���ڵĴ�Сδ��ʱ�ָ�������ֵ
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

	WindowPos = this->pos();			// ��¼��ǰ�������Ͻǵ���������
	PressedPos = event->globalPos();	// ��¼��갴�µ�λ��

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

	// ���δ����
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
	// �������
	if (hoverPos != Location::CENTER)
	{
		// �ڱ߽紦����
		QRect rect = this->rect();
		QPoint topLeft = mapToGlobal(rect.topLeft());
		QPoint bottomRight = mapToGlobal(rect.bottomRight());

		QRect rMove(topLeft, bottomRight);

		switch (hoverPos)
		{
		case Location::TOP:
			// ��ֹ���ڱ�����
			if(bottomRight.y() - globalPos.y() > this->minimumHeight())
				rMove.setY(globalPos.y());
			break;
		case Location::BOTTOM:
			rMove.setHeight(globalPos.y() - topLeft.y());
			break;
		case Location::LEFT:
			// ��ֹ���ڱ�����
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
		// �ڱ���������
		if (LeftPressedInTitle)
		{
			if (this->isMaximized())
			{
				// �������ʱ������϶�������
				// a.���ڻָ�
				double width_ratio = (double)event->pos().x() / width();
				qDebug() << width();
				this->setWindowState(Qt::WindowNoState);
				// b.�����Դ��ڵ�λ�ò���
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

// ˫��������
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

// ��ǰ���ڻ���ʱ���������е����пؼ��������׷�٣�����������ƶ������ڱ߽�ʱ������׷�ٵ���겢���ù��������״
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

	// �����ʵ�־���return false����ʾ���¼����Ŵ��ݡ�Ҳ���Ǵ��ݸ���Ӧ�Ŀؼ�
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
		// ���Ͻ�
		hoverPos = Location::TOP_LEFT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x() && y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// ���½�
		hoverPos = Location::BOTTOM_RIGHT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= topLeft.x() && x <= topLeft.x() + padding && y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// ���½�
		hoverPos = Location::BOTTOM_LEFT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x() && y >= topLeft.y() && y <= topLeft.y() + padding)
	{
		// ���Ͻ�
		hoverPos = Location::TOP_RIGHT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= topLeft.x() && x <= topLeft.x() + padding)
	{
		// ���
		hoverPos = Location::LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x >= bottomRight.x() - padding && x <= bottomRight.x())
	{
		// �ұ�
		hoverPos = Location::RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= topLeft.y() && y <= topLeft.y() + padding)
	{
		// �ϱ�
		hoverPos = Location::TOP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (y >= bottomRight.y() - padding && y <= bottomRight.y())
	{
		// �±�
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

		// ���������ʱ������������϶����ţ�������϶�������ʱ�����ڻָ�Ĭ�ϴ�С
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