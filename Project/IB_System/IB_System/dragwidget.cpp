#include "DragWidget.h"
#include <QFont>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QLabel>
#include <QDrag>
#include <QPainter>
#include <QDebug>

DragWidget::DragWidget(QWidget *parent)
    : QListWidget(parent)
    , m_curItem(nullptr)
{
    this->setViewMode(QListView::IconMode);
    this->setSpacing(10);
    this->setFixedSize(QSize(780, 250));
    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    this->setDropIndicatorShown(false);//显示拖放指示器
    QFont font;
//    font.setFamily(QString::fromLocal8Bit("微软雅黑"));
//    font.setPointSize(12);
    this->setFont(font);
}

DragWidget::~DragWidget()
{
    if(m_curItem)
        delete m_curItem;
}


void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else {
            event->acceptProposedAction();

        }
        //qDebug() << "dragEnter: "<<event->dropAction();
    }
    else {
        event->ignore();
    }
}
void DragWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            //qDebug() << "dragMove:   source  " << event->dropAction();

        }
        else {
            event->acceptProposedAction();
            //qDebug() << "dragMove:   nosource" << event->dropAction();
        }
    }
    else {
        event->ignore();
    }
}
void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QString txt = event->mimeData()->text();
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();

            QListWidgetItem *curItem = this->itemAt(event->pos());
            int iRow = this->row(curItem);
            if (iRow < 0)
                return;
            else {
                //m_curItem
                delete takeItem(row(m_curItem));
                this->insertItem(iRow, new QListWidgetItem(QIcon(pixmap), event->mimeData()->text()));
            }

            qDebug() << "dropEvent:   source" << event->dropAction();
        }
        else {
            this->addItem(new QListWidgetItem(QIcon(pixmap), event->mimeData()->text()));
            //event->acceptProposedAction();//建议的拖放动作
            //event->accept();
        }

    }
    else {
        event->ignore();
    }
}
void DragWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem *currentItem = this->itemAt(event->pos());
    m_curItem = this->itemAt(event->pos());
    if (!currentItem)
        return;
    QLabel *child = new QLabel;
    child->setGeometry(event->pos().x(), event->pos().y(), 50, 80);
    if (!child)
        return;

    QIcon curIcon = currentItem->icon();
    QPixmap pix = curIcon.pixmap(50, 80);
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pix << QPoint(event->pos() - child->pos());
    //! [1]

    //! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
    mimeData->setText(currentItem->text());
    //! [2]

    //! [3]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pix);
    drag->setHotSpot(event->pos()- child->pos()+QPoint(20,20));
    //! [3]

    QPixmap tempPixmap = pix;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pix.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);
    child->setText(currentItem->text());
    //参数1可能动作为移动或者复制,参数2建议动作为复制

    Qt::DropAction dropAction;
    dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction) {
        //delete takeItem(row(currentItem));
        return;
    }
    else if (dropAction == Qt::CopyAction) {
        //delete takeItem(row(currentItem));
    }
    else
    {
        if (drag->target())
            delete takeItem(row(currentItem));
    }
    qDebug() << "exec action" << dropAction;
    qDebug() << "drag target " << drag->target();
        //return;
}
