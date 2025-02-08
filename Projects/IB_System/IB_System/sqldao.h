#ifndef SQLDAO_H
#define SQLDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlRelationalDelegate>

#include "bus.h"
#include "line.h"
#include "role.h"
#include "station.h"
#include "user.h"
#include "scheduling.h"

class SqlDao : public QObject
{
    Q_OBJECT
public:

    static SqlDao *ptrSqlDao;
    static SqlDao *getinstance()
    {
        if(nullptr == ptrSqlDao)
            ptrSqlDao = new SqlDao;
        return ptrSqlDao;
    }

    explicit SqlDao(QObject *parent = nullptr);

    // 连接数据库
    void init();

    // 根据登录名判断是否存在该用户
    bool user_isExist(QString loginName);

    // 根据登录名获取密码
    QString get_loginName_password(QString loginName);

    // 判断车牌号是否已经存在
    bool buslicense_isExist(QString license);

    // 查寻所有站点名
    QStringList get_allStation(QString strSql);

signals:


private:
    QSqlDatabase m_db;

};

#endif // SQLDAO_H
