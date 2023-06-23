#include "line.h"

Line::Line(QObject *parent)
    : QObject{parent}
{

}

quint32 Line::getLineCode() const
{
    return lineCode;
}

void Line::setLineCode(quint32 newLineCode)
{
    lineCode = newLineCode;
}

QString Line::getLineName() const
{
    return lineName;
}

void Line::setLineName(const QString &newLineName)
{
    lineName = newLineName;
}

QString Line::getStatus() const
{
    return status;
}

void Line::setStatus(const QString &newStatus)
{
    status = newStatus;
}

QDateTime Line::getStartLineTime() const
{
    return startLineTime;
}

void Line::setStartLineTime(const QDateTime &newStartLineTime)
{
    startLineTime = newStartLineTime;
}

QString Line::getDirection() const
{
    return direction;
}

void Line::setDirection(const QString &newDirection)
{
    direction = newDirection;
}

QList<QString> Line::getAllStation() const
{
    return allStation;
}

void Line::setAllStation(const QList<QString> &newAllStation)
{
    allStation = newAllStation;
}
