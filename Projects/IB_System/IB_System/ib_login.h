#ifndef IB_LOGIN_H
#define IB_LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QFile>
#include <QRegularExpression>

#include "framelesswidget.h"
#include "sqldao.h"
#include "mydialog.h"
#include "ib_includestation.h"

namespace Ui {
class ib_Login;
}

class ib_Login : public QWidget
{
    Q_OBJECT

public:
    friend class ib_includeStation;
    explicit ib_Login(QWidget *parent = nullptr);
    ~ib_Login();

    QFont loadFont(QString name, int fontSize);

private slots:
    void on_btn_close_clicked();

    void on_btn_clear_clicked();

    void on_btn_login_clicked();

private:
    Ui::ib_Login *ui;
    FramelessWidget *frame;
    SqlDao *m_ptrSqlDao;
    MyDialog dialog;

signals:
    void sendLoginSuccess(QString loginName);

};

#endif // IB_LOGIN_H
