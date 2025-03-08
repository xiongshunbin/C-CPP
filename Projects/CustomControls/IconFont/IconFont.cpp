#include "IconFont.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(u8"图标字体");

    // 整体采用垂直布局
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    // 1.顶部栏
    QWidget* topWidget = new QWidget;
    QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
    topLayout->setSpacing(30);
    topLayout->setContentsMargins(0, 0, 0, 0);

    // 1.1字体库选择
    QGroupBox* fontGroup = new QGroupBox(this);
    fontGroup->setTitle(u8"图标字体库");
    QVBoxLayout* groupLayout = new QVBoxLayout(fontGroup);

    QString radioTexts[3] = { "FontAwesome4", "FontWeather", "FontAlibaba" };
    radioGroup = new QButtonGroup(this);

    for (int i = 0; i < 3; ++i)
    {
        QRadioButton* radio = new QRadioButton(this);
        radio->setText(radioTexts[i]);
        radio->setMinimumWidth(150);
        radioGroup->addButton(radio, i);
        groupLayout->addWidget(radio);

        connect(radio, &QRadioButton::clicked, this, &Widget::radioClicked);
    }

    topLayout->addWidget(fontGroup);

    // 1.2效果演示
    QGroupBox* showGroup = new QGroupBox(this);
    showGroup->setTitle(u8"效果演示");
	QHBoxLayout* showLayout = new QHBoxLayout(showGroup);
    showLayout->setSpacing(10);

    for (int i = 0; i < 7; ++i)
    {
        QLabel* lbl = new QLabel(this);
        lbl->setMinimumSize(100, 100);
        lbl->setAlignment(Qt::AlignCenter);

        showLayout->addWidget(lbl);

        lblList.append(lbl);
    }

    topLayout->addWidget(showGroup);

    // 1.3添加弹簧
    QSpacerItem* hspItem = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    topLayout->addItem(hspItem);

    vLayout->addWidget(topWidget);

    // 2.主体
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);      // 防止主体部分高度过小

    QWidget* scrollAreaContents = new QWidget(this);

    QVBoxLayout* verticalLayout = new QVBoxLayout(scrollAreaContents);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    QFrame* frame = new QFrame(scrollAreaContents);
    gridLayout = new QGridLayout(frame);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(frame);

	QSpacerItem* vspItem = new QSpacerItem(8, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);
    verticalLayout->addItem(vspItem);

    scrollArea->setWidget(scrollAreaContents);

    vLayout->addWidget(scrollArea);

    // FontAwesome4
    radioGroup->button(0)->click();
}

Widget::~Widget()
{}

void Widget::radioClicked()
{
    // 1.清空原来的标签
    int count = gridLayout->count();
    while (count--)
    {
        QWidget* widget = gridLayout->takeAt(count)->widget();
        gridLayout->removeWidget(widget);
        widget->setVisible(false);
        widget->deleteLater();      // free 
    }

    // 2.假设图标字体个数
    int checkedId = radioGroup->checkedId();
    if (checkedId == 0)             // FontAwesome4
    {
        count = 500;
    }
    else if (checkedId == 1)        // FontWeather
    {
        count = 200;
    }
    else if(checkedId == 2)         // FontAlibaba
    {
        count = 100;
    }

    // 3.显示图标(模拟 - 显示对应的索引)
    int row, column;
    for (int i = 0; i < count; ++i)
    {
        QLabel* lbl = new QLabel();

        lbl->setText(QString::number(i + 1));
        lbl->setMinimumSize(48, 48);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(R"(
            QLabel {
                color: #000000;
                border: 1px solid #000000;
                background-color: rgb(255, 255, 255);
            }            
            QLabel:hover {
                color: #ffffff;
                border: 1px solid #000000;
                background-color: rgb(0, 0, 0);
            }
        )");

        // 每行显示20个
        row = i / 20;
        column = i % 20;
        gridLayout->addWidget(lbl, row, column);

    }
}
