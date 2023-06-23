#ifndef IB_MAINWINDOW_H
#define IB_MAINWINDOW_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QDateTimeEdit>

#include "ib_login.h"
#include "ib_changepassword.h"
#include "myitemdelegate.h"
#include "readonly.h"
#include "mydialog.h"
#include "ib_includestation.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ib_mainWindow; }
QT_END_NAMESPACE

class ib_mainWindow : public QWidget
{
    Q_OBJECT

public:
    ib_mainWindow(QWidget *parent = nullptr);
    ~ib_mainWindow();

    void busDao(QString table);
    void lineDao(QString table);
    void userDao(QString table);
    void stationDao(QString table);
    void schedulingDao(QString table);



private slots:
    void on_btn_close_clicked();

    void on_btn_max_clicked();

    void on_btn_min_clicked();

    void Close();


    void on_btn_op_home_clicked();

    void on_btn_vehicle_clicked();

    void on_btn_station_clicked();

    void on_btn_route_clicked();

    void on_btn_schedule_clicked();

    void on_btn_staff_clicked();

    void on_btn_op_basicdata_clicked();

    void on_btn_op_workmanage_clicked();

    void on_btn_op_usermanage_clicked();

    void on_btn_chpass_clicked();

    void on_btn_addvehicle_clicked();

    void on_btn_savevehicle_clicked();

    void on_btn_delvehicle_clicked();

private:
    Ui::ib_mainWindow *ui;
    ib_Login Login;
    FramelessWidget *frame;
    QPropertyAnimation *in_animation;
    QPropertyAnimation *out_animation;
    ib_changePassword changePassword;
    ib_includeStation includeStation;

    QSqlTableModel *bus_model;
    QSqlTableModel *line_model;
    QSqlTableModel *station_model;
    QSqlRelationalTableModel *user_model;
    QSqlRelationalTableModel *scheduling_model;

    SqlDao *m_ptrSqlDao;

};
#endif // IB_MAINWINDOW_H
