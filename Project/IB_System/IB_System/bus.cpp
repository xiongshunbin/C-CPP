#include "bus.h"

Bus::Bus(QObject *parent)
    : QObject{parent}
{

}

quint32 Bus::getBusCode() const
{
    return busCode;
}

void Bus::setBusCode(quint32 newBusCode)
{
    busCode = newBusCode;
}

QString Bus::getBusLicense() const
{
    return busLicense;
}

void Bus::setBusLicense(const QString &newBusLicense)
{
    busLicense = newBusLicense;
}

QString Bus::getBusType() const
{
    return busType;
}

void Bus::setBusType(const QString &newBusType)
{
    busType = newBusType;
}

QString Bus::getBusStatus() const
{
    return busStatus;
}

void Bus::setBusStatus(const QString &newBusStatus)
{
    busStatus = newBusStatus;
}

QDateTime Bus::getStartTime() const
{
    return startTime;
}

void Bus::setStartTime(const QDateTime &newStartTime)
{
    startTime = newStartTime;
}

