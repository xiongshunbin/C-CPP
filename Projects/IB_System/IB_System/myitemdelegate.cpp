#include "myitemdelegate.h"

MyItemDelegate::MyItemDelegate()
{

}

MyItemDelegate::MyItemDelegate(QStringList list, QItemDelegate *parent) : QItemDelegate(parent)
{
    m_comboBoxList = list;
}

// 创建编辑器
QWidget *MyItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Q_UNUSED(option);
    Q_UNUSED(index);
    // 创建自己需要的控件进行返回
    QComboBox *editor = new QComboBox(parent);

    return editor;
}

// 设置编辑器数据
void MyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    Q_UNUSED(index);
    // 将参数editor转换为对应创建的控件，再进行数据初始设置就行
    QComboBox *cob = static_cast<QComboBox *>(editor);
    cob->addItems(m_comboBoxList);
}

// 更新编辑器集合属性
void MyItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    // 将编辑器设置为矩形属性
    editor->setGeometry(option.rect);
}

// 设置模型数据
void MyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    // 类型转换
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    // 模型（单元格）显示的数据
    model->setData(index, comboBox->currentText());
}
