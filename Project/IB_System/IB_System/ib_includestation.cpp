#include "ib_includestation.h"
#include "ui_ib_includestation.h"

ib_includeStation::ib_includeStation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ib_includeStation)
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
    f.setFileName(":/Styles/ib_includestation.css");
    f.open(QIODevice::ReadOnly);
    QString strQss = f.readAll();
    this->setStyleSheet(strQss);
    f.close();

    setWindowModality(Qt::ApplicationModal);

}

ib_includeStation::~ib_includeStation()
{
    delete ui;
}

QStringList ib_includeStation::get_include()
{
    QStringList result;
    for(int i = 0; i < ui->listWidget_modify->count(); i ++)
    {
        result.append(ui->listWidget_modify->item(i)->text());
    }
    return result;
}

void ib_includeStation::on_btn_close_clicked()
{
    this->close();
}


void ib_includeStation::on_btn_searchstation_clicked()
{
    ui->listWidget_result->clear();
    QString str_station = QString("s_name like '%%1%'").arg(ui->le_stationname->text());
    QString str_area = QString("s_location like '%%1%'").arg(ui->le_location->text());
    QString str_street = QString("s_street like '%%1%'").arg(ui->le_street->text());
    QString str = "";

    if(!ui->le_stationname->text().isEmpty())
        str.append(str_station);
    if(!ui->le_location->text().isEmpty())
    {
        if(!str.isEmpty())
            str.append(" and ");
        str.append(str_area);
    }
    if(!ui->le_street->text().isEmpty())
    {
        if(!str.isEmpty())
            str.append(" and ");
        str.append(str_street);
    }
    QString strSql;
    if(!str.isEmpty())
        strSql = QString("select s_name from station where " + str);
    else    strSql = "select s_name from station";
    QStringList l_allStation = SqlDao::ptrSqlDao->get_allStation(strSql);
    ui->listWidget_result->addItems(l_allStation);
}

void ib_includeStation::on_btn_confirm_clicked()
{
    this->close();
    emit sendConfirm();
}

