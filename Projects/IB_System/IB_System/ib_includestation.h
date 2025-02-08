#ifndef IB_INCLUDESTATION_H
#define IB_INCLUDESTATION_H

#include <QWidget>
#include <QFile>
#include <QDebug>

#include "framelesswidget.h"
#include "sqldao.h"

namespace Ui {
class ib_includeStation;
}

class ib_includeStation : public QWidget
{
    Q_OBJECT

public:
    explicit ib_includeStation(QWidget *parent = nullptr);
    ~ib_includeStation();

    QStringList get_include();

private slots:
    void on_btn_close_clicked();

    void on_btn_searchstation_clicked();

    void on_btn_confirm_clicked();

private:
    Ui::ib_includeStation *ui;

    FramelessWidget *frame;

signals:
    void sendConfirm();
};

#endif // IB_INCLUDESTATION_H
