#include "sqldao.h"

SqlDao *SqlDao::ptrSqlDao = nullptr;

SqlDao::SqlDao(QObject *parent)
    : QObject{parent}
{

}

void SqlDao::init()
{
    // 打开数据库
    m_db = QSqlDatabase::addDatabase("QMYSQL");

    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setDatabaseName("ib");
    m_db.setPassword("123456");
    m_db.setPort(3306);

    if(!m_db.open())
        qDebug() << "error: failed to connect the database!" << m_db.lastError().text();
    else
        qDebug() << "connect database successfully!";
}

bool SqlDao::user_isExist(QString loginName)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("select * from user where u_loginName = '%1'").arg(loginName);
    sql.exec(strSql);
    return sql.next();
}

QString SqlDao::get_loginName_password(QString loginName)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("select u_password from user where u_loginName = '%1'").arg(loginName);
    sql.exec(strSql);
    if(sql.next())  return sql.value(0).toString();
    else            return QString("");
}

bool SqlDao::buslicense_isExist(QString license)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("select * from bus where b_license = '%1'").arg(license);
    sql.exec(strSql);
    return sql.next();
}

QStringList SqlDao::get_allStation(QString strSql)
{
    QSqlQuery sql(m_db);
    QStringList result;
    sql.exec(strSql);
    while(sql.next())
    {
        result.append(sql.value(0).toString());
    }
    return result;
}
