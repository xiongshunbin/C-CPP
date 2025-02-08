#include "tidialog.h"
#include "ui_tidialog.h"

#include <QDebug>
#include <QMutex>
#include <QMessageBox>
#include <QPainter>

myDialog* myDialog::m_pInstance = NULL;
myDialog* myDialog::Instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_pInstance == NULL)
    {
        m_pInstance = new myDialog();
    }
    return m_pInstance;
}

void myDialog::Destory()
{
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//弹出消息框
void myDialog::showMessageBoxInfo(const QString &info,QWidget *parent,bool exec,const QString &confirmName)
{
//    ui->label_pix->setPixmap(QPixmap(":/icon-succeed.png"));
    showDialog("提示:"+info,parent,exec,confirmName,"取消");
}
//弹出错误框
void myDialog::showMessageBoxError(const QString &info,QWidget *parent,bool exec,const QString &confirmName)
{
//    ui->label_pix->setPixmap(QPixmap(":/icon-anomaly.png"));
    showDialog("错误:"+info,parent,exec,confirmName,"取消");
}
//弹出询问框
int myDialog::showMessageBoxQuestion(const QString &info,QWidget *parent,bool exec,const QString &confirmName, const QString &cancleName)
{
//    ui->label_pix->setPixmap(QPixmap(":/icon-succeed.png"));
    return showDialog(info,parent,exec,confirmName,cancleName);
}

int myDialog::showDialog(const QString &info, QWidget *parent, bool exec, const QString &confirmName, const QString &cancleName)
{
    if(info == m_Message)
        return QMessageBox::Cancel;
    if(exec && !this->isHidden())
    {
        qDebug() << "showMessageBoxInfo 已经有一个了";
        myDialog *dlg = new myDialog();
        return dlg->showMessageBoxQuestion(info,parent,exec);
    }
    setParent(parent);
    m_Message = info;
    ui->pushButton_confirm->setText(confirmName);
    ui->pushButton_cancle->setText(cancleName);
    ui->pushButton_confirm->show();
    ui->pushButton_cancle->show();

    ui->label_Tip->setText(info);
    update();
    if(exec)
    {
        m_Message.clear();
        QtMaterialJbyDialog::exec();
    }
    else
    {
        QtMaterialJbyDialog::showDialog();
    }
    return DlgState();
}




myDialog::myDialog(QWidget *parent) :
    QtMaterialJbyDialog(parent),
    ui(new Ui::myDialog)
{
    ui->setupUi(this);
    ui->widget_main->setStyleSheet("QWidget#widget_main{  background:rgb(227, 227, 227);border: 1px solid #F1F1F1; }");

//    ui->pushButton_cancle->setStyleSheet("min-height:40px;max-height:60px;  min-width:70px; max-width:150px;");
//    ui->pushButton_confirm->setStyleSheet("min-height:40px;max-height:60px;  min-width:70px; max-width:150px;");
//    ui->widget_main->setMinimumSize(1300,427);

    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->label_pix->setScaledContents(true);
    //QtMaterialJbyDialog 使用方法  1.设置动画窗口 2.设置动画方向 3.设置父类
    QtMaterialJbyDialog::setMainWidget(ui->widget_main);
    QtMaterialJbyDialog::setAnimationDirection(QtMaterialJbyDialog::UPTODOWN);
}


myDialog::~myDialog()
{
    delete ui;
}


//取消
void myDialog::on_pushButton_cancle_clicked()
{
    setDlgState(QMessageBox::No);
    closeDialog();
}
//确定
void myDialog::on_pushButton_confirm_clicked()
{
    setDlgState(QMessageBox::Yes);
    closeDialog();
}

void myDialog::on_btn_close_clicked()
{
    this->close();
}

