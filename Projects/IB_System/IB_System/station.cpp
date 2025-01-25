#include "station.h"

Station::Station(QWidget *parent)
    : QWidget{parent}
{

}

quint32 Station::getStationCode() const
{
    return stationCode;
}

void Station::setStationCode(quint32 newStationCode)
{
    stationCode = newStationCode;
}

QString Station::getStationName() const
{
    return stationName;
}

void Station::setStationName(const QString &newStationName)
{
    stationName = newStationName;
}

QString Station::getLongitude() const
{
    return longitude;
}

void Station::setLongitude(const QString &newLongitude)
{
    longitude = newLongitude;
}

QString Station::getLatitude() const
{
    return latitude;
}

void Station::setLatitude(const QString &newLatitude)
{
    latitude = newLatitude;
}

QString Station::getRegion() const
{
    return region;
}

void Station::setRegion(const QString &newRegion)
{
    region = newRegion;
}

QString Station::getStreet() const
{
    return street;
}

void Station::setStreet(const QString &newStreet)
{
    street = newStreet;
}
