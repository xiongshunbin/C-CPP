#include "ib_includestation.h"
#include "ui_ib_includestation.h"

ib_includeStation::ib_includeStation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ib_includeStation)
{
    ui->setupUi(this);

    // 去掉系统边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 实现无边框窗口的移动
    frame= new FramelessWidget(this);
    frame->setPadding(5);
    frame->setMoveEnable(true);
    frame->setResizeEnable(false);
    frame->setWidget(this);

    QFile f;
    f.setFileName(":/Styles/ib_includestation.css");
    f.open(QIODevice::ReadOnly);
    QString strQss = f.readAll();
    this->setStyleSheet(strQss);
    f.close();

    setWindowModality(Qt::ApplicationModal);
}

ib_includeStation::~ib_includeStation()
{
    delete ui;
}

void ib_includeStation::on_btn_close_clicked()
{
    this->close();
}

