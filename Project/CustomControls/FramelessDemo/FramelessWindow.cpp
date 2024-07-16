#include "FramelessWindow.h"
#include <QDebug>

FramelessWindow::FramelessWindow(QWidget* parent) : QWidget(parent)
{
	// ȥ��������
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	this->installEventFilter(this);
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
		if (this->windowState().testFlag(Qt::WindowNoState))
		{
			setCursorShape(globalPos);
		}
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
	else if (LeftPressedInTitle)
	{
		// �ڱ���������
		this->move(WindowPos + event->globalPos() - PressedPos);
	}
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent* event)
{
	LeftPressed = false;
	LeftPressedInTitle = false;
	this->setCursor(QCursor(Qt::ArrowCursor));
}


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
