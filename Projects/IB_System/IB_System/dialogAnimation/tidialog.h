#ifndef TIDIALOG_H
#define TIDIALOG_H

#include <dialogAnimation/qtmaterialjbydialog.h>


namespace Ui {
class myDialog;
}

class myDialog : public QtMaterialJbyDialog
{
    Q_OBJECT

public:
    explicit myDialog(QWidget *parent = nullptr);
    ~myDialog();
    static    myDialog* m_pInstance;
    static    myDialog *Instance();
    static    void Destory();

    void overlayAnimation();

    //弹出消息框
    void showMessageBoxInfo(const QString &info,QWidget *parent,bool exec = true,const QString &confirmName="确 定");
    //弹出错误框
    void showMessageBoxError(const QString &info,QWidget *parent,bool exec = true,const QString &confirmName="确 定");
    //弹出询问框
    int showMessageBoxQuestion(const QString &info,QWidget *parent,bool exec = true,const QString &confirmName="确 定", const QString &cancleName="取 消");

private:
    int showDialog(const QString &info,QWidget *parent,bool exec = true,const QString &confirmName="确 定", const QString &cancleName="取 消");

private slots:
    void on_pushButton_cancle_clicked();

    void on_pushButton_confirm_clicked();

    void on_btn_close_clicked();

private:
    Ui::myDialog *ui;
    QString m_Message;  //当前显示值

};

#endif // TIDIALOG_H
