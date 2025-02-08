#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

class MyItemDelegate : public QItemDelegate
{
public:
    MyItemDelegate();
    MyItemDelegate (QStringList list, QItemDelegate *parent = nullptr);

    // 创建编辑器
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 设置编辑器数据
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    // 更新编辑器集合属性
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 设置模型数据
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    // 存储QComBoBox的数据
    QStringList m_comboBoxList;

};

#endif // MYITEMDELEGATE_H
