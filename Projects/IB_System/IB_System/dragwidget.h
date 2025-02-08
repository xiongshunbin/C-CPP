#pragma once

#include <QWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE


class DragWidget : public QListWidget
{
    Q_OBJECT

public:
    DragWidget(QWidget *parent = 0);
    ~DragWidget();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QListWidgetItem *m_curItem;

};
