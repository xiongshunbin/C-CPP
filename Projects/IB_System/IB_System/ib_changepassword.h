#ifndef IB_CHANGEPASSWORD_H
#define IB_CHANGEPASSWORD_H

#include <QWidget>
#include <QFile>

#include "framelesswidget.h"

namespace Ui {
class ib_changePassword;
}

class ib_changePassword : public QWidget
{
    Q_OBJECT

public:
    explicit ib_changePassword(QWidget *parent = nullptr);
    ~ib_changePassword();


private slots:
    void on_btn_close_clicked();

    void on_btn_clo_clicked();

private:
    Ui::ib_changePassword *ui;
    FramelessWidget *frame;
};

#endif // IB_CHANGEPASSWORD_H
