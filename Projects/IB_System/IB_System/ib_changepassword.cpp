#include "ib_changepassword.h"
#include "ui_ib_changepassword.h"

ib_changePassword::ib_changePassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ib_changePassword)
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
    f.setFileName(":/Styles/mydialog.css");
    f.open(QIODevice::ReadOnly);
    QString strQss = f.readAll();
    this->setStyleSheet(strQss);
    f.close();

    setWindowModality(Qt::ApplicationModal);
}

ib_changePassword::~ib_changePassword()
{
    delete ui;
}


void ib_changePassword::on_btn_close_clicked()
{
    this->close();
    ui->le_old->clear();
    ui->le_new->clear();
    ui->le_confirm->clear();
}


void ib_changePassword::on_btn_clo_clicked()
{
    this->close();
    ui->le_old->clear();
    ui->le_new->clear();
    ui->le_confirm->clear();
}

