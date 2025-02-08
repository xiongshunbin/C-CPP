#include "scheduling.h"

Scheduling::Scheduling(QObject *parent)
    : QObject{parent}
{

}

quint32 Scheduling::getCode() const
{
    return code;
}

void Scheduling::setCode(quint32 newCode)
{
    code = newCode;
}

quint32 Scheduling::getLineCode() const
{
    return lineCode;
}

void Scheduling::setLineCode(quint32 newLineCode)
{
    lineCode = newLineCode;
}

QString Scheduling::getTcNumber() const
{
    return tcNumber;
}

void Scheduling::setTcNumber(const QString &newTcNumber)
{
    tcNumber = newTcNumber;
}

QString Scheduling::getTcTime() const
{
    return tcTime;
}

void Scheduling::setTcTime(const QString &newTcTime)
{
    tcTime = newTcTime;
}

quint32 Scheduling::getUserCode() const
{
    return userCode;
}

void Scheduling::setUserCode(quint32 newUserCode)
{
    userCode = newUserCode;
}

QString Scheduling::getStartStation() const
{
    return startStation;
}

void Scheduling::setStartStation(const QString &newStartStation)
{
    startStation = newStartStation;
}

QString Scheduling::getEndStation() const
{
    return endStation;
}

void Scheduling::setEndStation(const QString &newEndStation)
{
    endStation = newEndStation;
}

quint32 Scheduling::getBusCode() const
{
    return busCode;
}

void Scheduling::setBusCode(quint32 newBusCode)
{
    busCode = newBusCode;
}

QString Scheduling::getLineName() const
{
    return lineName;
}

void Scheduling::setLineName(const QString &newLineName)
{
    lineName = newLineName;
}

QString Scheduling::getUserName() const
{
    return userName;
}

void Scheduling::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

QString Scheduling::getBusLicense() const
{
    return busLicense;
}

void Scheduling::setBusLicense(const QString &newBusLicense)
{
    busLicense = newBusLicense;
}
