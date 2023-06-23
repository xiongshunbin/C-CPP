#include "ib_mainwindow.h"
#include "ui_ib_mainwindow.h"
#include <QPropertyAnimation>
#include <QFontDatabase>


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

    connect(ui->tableView_route,&QTableView::doubleClicked, this,[=](const QModelIndex &index){
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
    ui->tableView_station->setItemDelegateForColumn(0, readOnlyDelegate); // 设置第一列不可更改

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
}


void ib_mainWindow::on_btn_station_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(0);
    ui->tw_basicdata->setCurrentIndex(1);
}


void ib_mainWindow::on_btn_route_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(0);
    ui->tw_basicdata->setCurrentIndex(2);
}


void ib_mainWindow::on_btn_schedule_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(1);
    ui->tw_basicdata->setCurrentIndex(0);
}


void ib_mainWindow::on_btn_staff_clicked()
{
    ui->ib_menu->setCurrentIndex(1);
    ui->content->setCurrentIndex(2);
    ui->tw_basicdata->setCurrentIndex(0);
}


void ib_mainWindow::on_btn_op_basicdata_clicked()
{
    ui->content->setCurrentIndex(0);
}


void ib_mainWindow::on_btn_op_workmanage_clicked()
{
    ui->content->setCurrentIndex(1);
}


void ib_mainWindow::on_btn_op_usermanage_clicked()
{
    ui->content->setCurrentIndex(2);
}


void ib_mainWindow::on_btn_chpass_clicked()
{
    changePassword.show();
}

void ib_mainWindow::on_btn_addvehicle_clicked()
{
    int rowNum = bus_model->rowCount();//获得表的行数
    bus_model->insertRow(rowNum); //添加一行
}


void ib_mainWindow::on_btn_savevehicle_clicked()
{
    bus_model->submitAll();
}


void ib_mainWindow::on_btn_delvehicle_clicked()
{
    int curRow = ui->tableView_vehicle->currentIndex().row();
    bus_model->removeRow(curRow);

}

