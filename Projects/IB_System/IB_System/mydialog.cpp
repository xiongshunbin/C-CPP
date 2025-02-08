#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
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

}


MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::setInformation(QString text)
{
    ui->text_information->setText(text);
    QTextDocument *doc = ui->text_information->document();
    doc->adjustSize();

    int width = doc->size().toSize().width()+30;
    int height = doc->size().toSize().height()+10;

    width = width < 400 ? 400 : width;
    height = height < 136 ? 136 : height;

    this->setFixedSize(width, height);
}

void MyDialog::setPix(QString text, QString path)
{
    ui->lb_info->setText(text);
    ui->llb_error->setPixmap(path);
}

void MyDialog::on_btn_close_clicked()
{
    this->close();
}


void MyDialog::on_btn_ok_clicked()
{
    accept();
}

