#ifndef IB_INCLUDESTATION_H
#define IB_INCLUDESTATION_H

#include <QWidget>
#include <QFile>

#include "framelesswidget.h"

namespace Ui {
class ib_includeStation;
}

class ib_includeStation : public QWidget
{
    Q_OBJECT

public:
    explicit ib_includeStation(QWidget *parent = nullptr);
    ~ib_includeStation();

private slots:
    void on_btn_close_clicked();

private:
    Ui::ib_includeStation *ui;

    FramelessWidget *frame;
};

#endif // IB_INCLUDESTATION_H
