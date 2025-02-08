#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QFile>
#include "framelesswidget.h"

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();

    void setInformation(QString text);
    void setPix(QString text, QString path);

private slots:
    void on_btn_close_clicked();

    void on_btn_ok_clicked();

private:
    Ui::MyDialog *ui;
    FramelessWidget *frame;
};

#endif // MYDIALOG_H
