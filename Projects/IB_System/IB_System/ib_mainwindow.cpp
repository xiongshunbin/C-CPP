#include "ib_mainwindow.h"
#include "ui_ib_mainwindow.h"
#include <QPropertyAnimation>
#include <QFontDatabase>
#include "dialogAnimation/tidialog.h"


ib_mainWindow::ib_mainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ib_mainWindow)
{
    ui->setupUi(this);
    Login.show();

    // 去掉系统边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 实现无边框窗口的拖拽和移动
    frame= new FramelessWidget(this);
    frame->setPadding(5);
    frame->setMoveEnable(true);
    frame->setResizeEnable(true);
    frame->setWidget(this);

    in_animation = new QPropertyAnimation(this, "windowOpacity");
    in_animation->setDuration(500);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);


    connect(&Login, &ib_Login::sendLoginSuccess, this, [&](QString loginName){
        in_animation->start();
        this->show();
        ui->ib_menu->setCurrentIndex(0);
        ui->lb_user_name->setText(loginName);
        Login.close();
    });

    QFile file;
    file.setFileName(":/Styles/ib_mainwindow.css");
    file.open(QIODevice::ReadOnly);
    QString strQss = file.readAll();
    this->setStyleSheet(strQss);
    file.close();

    out_animation = new QPropertyAnimation(this, "windowOpacity");
    out_animation->setDuration(500);
    out_animation->setStartValue(1);
    out_animation->setEndValue(0);

    connect(out_animation, SIGNAL(finished()), this, SLOT(Close()));

    bus_model = new QSqlTableModel(this);
    line_model = new QSqlTableModel(this);
    station_model = new QSqlTableModel(this);
    user_model = new QSqlRelationalTableModel(this);
    scheduling_model = new QSqlRelationalTableModel(this);

    busDao("bus");
    lineDao("line");
    stationDao("station");
    userDao("user");
    schedulingDao("scheduling");

    m_contextMenu = new QMenu;
    m_refresh = new QAction("刷新", this);
    m_contextMenu->addAction(m_refresh);
    connect(m_refresh, SIGNAL(triggered(bool)), this, SLOT(refresh()));

}

ib_mainWindow::~ib_mainWindow()
{
    delete ui;
}

void ib_mainWindow::busDao(QString table)
{
    ui->tableView_vehicle->setModel(bus_model);
    ui->tableView_vehicle->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vehicle->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_vehicle->setSortingEnabled(true);
    bus_model->setTable(table);
    bus_model->select();

    bus_model->setHeaderData(0, Qt::Horizontal, "车辆编号");
    bus_model->setHeaderData(1, Qt::Horizontal, "车牌号");
    bus_model->setHeaderData(2, Qt::Horizontal, "车型");
    bus_model->setHeaderData(3, Qt::Horizontal, "车辆状态");
    bus_model->setHeaderData(4, Qt::Horizontal, "起运日期");

    ui->tableView_vehicle->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter);

    QStringList list;
    list << "启用" << "停运";
    MyItemDelegate* m_cBoxDelegate = new MyItemDelegate(list);
    ui->tableView_vehicle->setItemDelegateForColumn(3, m_cBoxDelegate);

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    ui->tableView_vehicle->setItemDelegateForColumn(0, readOnlyDelegate); // 设置第一列不可更改

    bus_model->setEditStrategy(QSqlTableModel::OnManualSubmit);            // 设置mod的编辑模式，手动提交修改
}

void ib_mainWindow::lineDao(QString table)
{
    ui->tableView_route->setModel(line_model);
    ui->tableView_route->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_route->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_route->setSortingEnabled(true);
    line_model->setTable(table);
    line_model->select();

    line_model->setHeaderData(0, Qt::Horizontal, "线路编号");
    line_model->setHeaderData(1, Qt::Horizontal, "线路名称");
    line_model->setHeaderData(2, Qt::Horizontal, "线路状态");
    line_model->setHeaderData(3, Qt::Horizontal, "开线日期");
    line_model->setHeaderData(4, Qt::Horizontal, "线路方向");
    line_model->insertColumn(5);
    line_model->setHeaderData(5, Qt::Horizontal, "包含站点");

    ui->tableView_route->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter);

    QStringList list;
    list << "启用" << "停运";
    MyItemDelegate* m_cBoxDelegate1 = new MyItemDelegate(list);
    ui->tableView_route->setItemDelegateForColumn(2, m_cBoxDelegate1);

    list.clear();
    list << "上行" << "下行";
    MyItemDelegate* m_cBoxDelegate2 = new MyItemDelegate(list);
    ui->tableView_route->setItemDelegateForColumn(4, m_cBoxDelegate2);

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    ui->tableView_route->setItemDelegateForColumn(0, readOnlyDelegate); // 设置第一列不可更改

    line_model->setEditStrategy(QSqlTableModel::OnManualSubmit);            // 设置mod的编辑模式，手动提交修改

    // 通过拖拉选择包含站点
    connect(ui->tableView_route,&QTableView::doubleClicked, this,[&](const QModelIndex &index){
        if(index.column() == 5)
        {
            includeStation.show();
        }
    });
}

void ib_mainWindow::userDao(QString table)
{
    user_model->setTable(table);
    user_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    user_model->setRelation(6, QSqlRelation("role", "r_id", "r_name"));

    user_model->setHeaderData(0, Qt::Horizontal, "员工编号");
    user_model->setHeaderData(1, Qt::Horizontal, "登录名");
    user_model->setHeaderData(2, Qt::Horizontal, "密码");
    user_model->setHeaderData(3, Qt::Horizontal, "姓名");
    user_model->setHeaderData(4, Qt::Horizontal, "手机号");
    user_model->setHeaderData(5, Qt::Horizontal, "身份证号");
    user_model->setHeaderData(6, Qt::Horizontal, "角色");
    user_model->setHeaderData(7, Qt::Horizontal, "驾龄");
    user_model->setHeaderData(8, Qt::Horizontal, "用户状态");

    user_model->select();
    ui->tableView_user->setModel(user_model);
    ui->tableView_user->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_user->setSelectionBehavior(QAbstractItemView::SelectRows);    // 设置选择整行
    ui->tableView_user->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_user));
    ui->tableView_user->hideColumn(2);
    ui->tableView_user->setSortingEnabled(true);
    ui->tableView_user->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter); // 表头居左对齐

    QStringList list;
    list << "启用" << "禁用";
    MyItemDelegate* m_cBoxDelegate = new MyItemDelegate(list);
    ui->tableView_user->setItemDelegateForColumn(8, m_cBoxDelegate);        // 添加下拉可选框

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    ui->tableView_user->setItemDelegateForColumn(0, readOnlyDelegate); // 设置第一列不可更改
}

void ib_mainWindow::stationDao(QString table)
{
    ui->tableView_station->setModel(station_model);
    ui->tableView_station->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_station->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_station->setSortingEnabled(true);
    station_model->setTable(table);
    station_model->select();

    station_model->setHeaderData(0, Qt::Horizontal, "站点编号");
    station_model->setHeaderData(1, Qt::Horizontal, "站点名称");
    station_model->setHeaderData(2, Qt::Horizontal, "经度");
    station_model->setHeaderData(3, Qt::Horizontal, "纬度");
    station_model->setHeaderData(4, Qt::Horizontal, "所在区域");
    station_model->setHeaderData(5, Qt::Horizontal, "所在街道");

    ui->tableView_station->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter);

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    ui->tableView_station->setItemDelegateForColumn(0, readOnlyDelegate);       // 设置第一列不可更改

    station_model->setEditStrategy(QSqlTableModel::OnManualSubmit);            // 设置mod的编辑模式，手动提交修改
}

void ib_mainWindow::schedulingDao(QString table)
{
    scheduling_model->setTable(table);
    scheduling_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    scheduling_model->setRelation(1, QSqlRelation("line", "l_id", "l_name"));
    scheduling_model->setRelation(5, QSqlRelation("user", "u_id", "u_name"));
    scheduling_model->setRelation(6, QSqlRelation("station", "s_id", "s_name"));
    scheduling_model->setRelation(7, QSqlRelation("station", "s_id", "s_name"));
    scheduling_model->setRelation(2, QSqlRelation("bus", "b_license", "b_license"));


    scheduling_model->setHeaderData(0, Qt::Horizontal, "排班编号");
    scheduling_model->setHeaderData(1, Qt::Horizontal, "线路名称");
    scheduling_model->setHeaderData(2, Qt::Horizontal, "车辆信息");
    scheduling_model->setHeaderData(3, Qt::Horizontal, "趟次");
    scheduling_model->setHeaderData(4, Qt::Horizontal, "每趟时间");
    scheduling_model->setHeaderData(5, Qt::Horizontal, "司机姓名");
    scheduling_model->setHeaderData(6, Qt::Horizontal, "始发站点");
    scheduling_model->setHeaderData(7, Qt::Horizontal, "终点站点");

    scheduling_model->select();
    ui->tableView_schedule->setModel(scheduling_model);
    ui->tableView_schedule->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_schedule->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_schedule->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_schedule));
    ui->tableView_schedule->setSortingEnabled(true);
    ui->tableView_schedule->horizontalHeader()->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter);

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    ui->tableView_schedule->setItemDelegateForColumn(0, readOnlyDelegate); // 设置第一列不可更改
}

void ib_mainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //把鼠标位置转化成相对于QWidget的位置，然后判断是否在ib_menu内
    QPoint i = ui->ib_menu->mapFromGlobal(event->globalPos());
    QRect rect(QPoint(0,0),ui->ib_menu->size());
    if (rect.contains(i) && ui->ib_menu->currentIndex() == 1)
    {
        m_contextMenu->exec(event->globalPos());
    }
}


void ib_mainWindow::on_btn_close_clicked()
{
    out_animation->start();
}


void ib_mainWindow::on_btn_max_clicked()
{
    if(this->isMaximized())
    {
        this->showNormal();
        frame->setMoveEnable(true);
        frame->setResizeEnable(true);
    }
    else
    {
        this->showMaximized();
        frame->setMoveEnable(false);
        frame->setResizeEnable(false);
    }
}


void ib_mainWindow::on_btn_min_clicked()
{
    this->showMinimized();
}

void ib_mainWindow::Close()
{
    this->close();
}

void ib_mainWindow::on_btn_op_home_clicked()
{
    ui->ib_menu->setCurrentIndex(0);
}


void ib_mainWindow::on_btn_vehicle_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(0);
    ui->tw_basicdata->setCurrentIndex(0);
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_station_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(0);
    ui->tw_basicdata->setCurrentIndex(1);
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_route_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(0);
    ui->tw_basicdata->setCurrentIndex(2);
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_schedule_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(1);
    ui->tw_basicdata->setCurrentIndex(0);
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_staff_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(2);
    ui->tw_basicdata->setCurrentIndex(0);
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_op_basicdata_clicked()
{
    ui->content->setCurrentIndex(0);
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_op_workmanage_clicked()
{
    ui->content->setCurrentIndex(1);
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_op_usermanage_clicked()
{
    ui->content->setCurrentIndex(2);
    ui->btn_op_usermanage->setStyleSheet("border: none;font-weight: bold;color: rgb(51, 102, 153);");
    ui->btn_op_basicdata->setStyleSheet("border: none;font-weight: bold;color: black;");
    ui->btn_op_workmanage->setStyleSheet("border: none;font-weight: bold;color: black;");
}


void ib_mainWindow::on_btn_chpass_clicked()
{
    changePassword.show();
}

void ib_mainWindow::on_btn_addvehicle_clicked()
{
    int rowNum = bus_model->rowCount();//获得表的行数
    bus_model->insertRow(rowNum); //添加一行
    ui->tableView_vehicle->model()->setData(ui->tableView_vehicle->model()->index(rowNum, 3), "启用");
}


void ib_mainWindow::on_btn_savevehicle_clicked()
{
    // 车牌号正则表达式
    static QRegularExpression reg_buslicense("^[\u4e00-\u9fa5]{1}[A-Z]{1}-[A-Z_0-9]{5}$");
    QRegularExpressionMatch match_buslicense;

    for(int i = 0; i < ui->tableView_vehicle->model()->rowCount(); i ++)
    {
        QString str_buslicense = ui->tableView_vehicle->model()->index(i, 1).data().toString();
        match_buslicense = reg_buslicense.match(str_buslicense);
        if(!match_buslicense.hasMatch())
        {
            dialog.setPix("错误", ":/Images/warn.png");
            dialog.setInformation("车牌号为空或格式不正确!");
            dialog.setFixedSize(272, 136);
            dialog.show();
            ui->tableView_vehicle->selectRow(i);
            return;
        }
        if(ui->tableView_vehicle->model()->index(i, 2).data().toString().isEmpty())
        {
            dialog.setPix("提示", ":/Images/remind.png");
            dialog.setInformation("车型不能为空!");
            dialog.setFixedSize(272, 136);
            dialog.show();
            ui->tableView_vehicle->setCurrentIndex(bus_model->index(i, 0));
            return;
        }
    }
    if(bus_model->submitAll())
    {
        dialog.setPix("提示", ":/Images/success.png");
        dialog.setInformation("操作成功");
        dialog.setFixedSize(272, 136);
        dialog.show();
    }
    else
    {
        dialog.setPix("错误", ":/Images/warn.png");
        dialog.setInformation("车牌号重复!");
        dialog.setFixedSize(272, 136);
        dialog.show();
        return;
    }
}


void ib_mainWindow::on_btn_delvehicle_clicked()
{
    int curRow = ui->tableView_vehicle->currentIndex().row();
    bus_model->removeRow(curRow);

    myDialog::Instance()->setAnimationDirection(QtMaterialJbyDialog::UPTODOWN);
    int dlg = myDialog::Instance()->showMessageBoxQuestion("确定要删除此行吗？",this,true,"确定","取消");
    if(dlg == QMessageBox::Yes)
    {
        bus_model->submitAll();
    }
    if(dlg == QMessageBox::No)
    {
        bus_model->revertAll();
    }
}

void ib_mainWindow::refresh()
{
    bus_model->select();
    line_model->select();
    station_model->select();
    scheduling_model->select();
    user_model->select();
}

void ib_mainWindow::on_btn_searchvehicle_clicked()
{
    QString str_buslicense = QString("b_license like '%%1%'").arg(ui->le_vehiclenumber->text());
    QString str_busstatus = QString("b_status = '%1'").arg(ui->cb_vehiclestate->currentText());
    QString str = "";
    if(!ui->le_vehiclenumber->text().isEmpty())
        str.append(str_buslicense);
    if(ui->cb_vehiclestate->currentText() != "全部")
    {
        if(!str.isEmpty())
            str.append(" and ");
        str.append(str_busstatus);
    }
    bus_model->setFilter(str);
    bus_model->select();
}


void ib_mainWindow::on_btn_addstation_clicked()
{
    int rowNum = station_model->rowCount();//获得表的行数
    station_model->insertRow(rowNum); //添加一行
}


void ib_mainWindow::on_btn_delstation_clicked()
{
    int curRow = ui->tableView_station->currentIndex().row();
    station_model->removeRow(curRow);

    myDialog::Instance()->setAnimationDirection(QtMaterialJbyDialog::UPTODOWN);
    int dlg = myDialog::Instance()->showMessageBoxQuestion("确定要删除此行吗？",this,true,"确定","取消");
    if(dlg == QMessageBox::Yes)
    {
        station_model->submitAll();
    }
    if(dlg == QMessageBox::No)
    {
        station_model->revertAll();
    }
}


void ib_mainWindow::on_btn_savestation_clicked()
{
    // reg_longitude : 经度正则表达式 -180.0～+180.0（整数部分为0～180，必须输入1到5位小数）
    static QRegularExpression reg_longitude("^[\\-\\+]?(0?\\d{1,2}\\.\\d{1,5}|1[0-7]?\\d{1}\\.\\d{1,5}|180\\.0{1,5})$");
    QRegularExpressionMatch match_reg_longitude;

    // reg_latitude : 纬度正则表达式 -90.0～+90.0（整数部分为0～90，必须输入1到5位小数）
    static QRegularExpression reg_latitude("^[\\-\\+]?([0-8]?\\d{1}\\.\\d{1,5}|90\\.0{1,5})$");
    QRegularExpressionMatch match_reg_latitude;

    for(int i = 0; i < ui->tableView_station-> model()->rowCount(); i ++)
    {
        QString str_station = ui->tableView_station->model()->index(i, 1).data().toString();
        if(str_station.length() <= 0 || str_station.length() >= 50)
        {
            dialog.setPix("错误", ":/Images/remind.png");
            dialog.setInformation("站点名不能为空，且最多50个字符!");
            dialog.setFixedSize(350, 136);
            dialog.show();
            ui->tableView_station->selectRow(i);
            return;
        }

        QString str_longitude = ui->tableView_station->model()->index(i, 2).data().toString();
        match_reg_longitude = reg_longitude.match(str_longitude);
        if(!match_reg_longitude.hasMatch())
        {
            dialog.setPix("错误", ":/Images/remind.png");
            dialog.setInformation("站点经度为空或格式不正确!");
            dialog.setFixedSize(300, 136);
            dialog.show();
            ui->tableView_station->selectRow(i);
            return;
        }

        QString str_latitude = ui->tableView_station->model()->index(i, 3).data().toString();
        match_reg_latitude = reg_latitude.match(str_latitude);
        if(!match_reg_latitude.hasMatch())
        {
            dialog.setPix("错误", ":/Images/remind.png");
            dialog.setInformation("站点维度为空或格式不正确!");
            dialog.setFixedSize(300, 136);
            dialog.show();
            ui->tableView_station->selectRow(i);
            return;
        }

        QString str_area = ui->tableView_station->model()->index(i, 4).data().toString();
        if(str_area.length() <= 0 || str_area.length() >= 50)
        {
            dialog.setPix("错误", ":/Images/remind.png");
            dialog.setInformation("区域不能为空，且最多50个字符!");
            dialog.setFixedSize(350, 136);
            dialog.show();
            ui->tableView_station->selectRow(i);
            return;
        }

        QString str_street = ui->tableView_station->model()->index(i, 5).data().toString();
        if(str_street.length() <= 0 || str_street.length() >= 50)
        {
            dialog.setPix("错误", ":/Images/remind.png");
            dialog.setInformation("街道不能为空，且最多50个字符!");
            dialog.setFixedSize(350, 136);
            dialog.show();
            ui->tableView_station->selectRow(i);
            return;
        }
    }
    if(station_model->submitAll())
    {
        dialog.setPix("提示", ":/Images/success.png");
        dialog.setInformation("操作成功");
        dialog.setFixedSize(272, 136);
        dialog.show();
    }
    else
    {
        station_model->revertAll();
    }
}


void ib_mainWindow::on_btn_searchstation_clicked()
{
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
    station_model->setFilter(str);
    station_model->select();
}


void ib_mainWindow::on_btn_addroute_clicked()
{
    int rowNum = line_model->rowCount();//获得表的行数
    line_model->insertRow(rowNum); //添加一行
}


void ib_mainWindow::on_btn_delroute_clicked()
{

}


void ib_mainWindow::on_btn_saveroute_clicked()
{

}


void ib_mainWindow::on_btn_searchroute_clicked()
{

}
