#ifndef READONLY_H
#define READONLY_H


#include <QItemDelegate>

// 实现一个委托（delegate），然后设设置为只读列
class ReadOnlyDelegate: public QItemDelegate
{

public:
    ReadOnlyDelegate(QWidget *parent = NULL):QItemDelegate(parent)
    {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
const QModelIndex &index) const override //final
    {
        Q_UNUSED(parent);
        Q_UNUSED(option);
        Q_UNUSED(index);
        return NULL;
    }
};
#endif // READONLY_H
