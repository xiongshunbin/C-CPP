#ifndef STATION_H
#define STATION_H

#include <QWidget>

class Station : public QWidget
{
    Q_OBJECT
public:
    explicit Station(QWidget *parent = nullptr);

    quint32 getStationCode() const;
    void setStationCode(quint32 newStationCode);

    QString getStationName() const;
    void setStationName(const QString &newStationName);

    QString getLongitude() const;
    void setLongitude(const QString &newLongitude);

    QString getLatitude() const;
    void setLatitude(const QString &newLatitude);

    QString getRegion() const;
    void setRegion(const QString &newRegion);

    QString getStreet() const;
    void setStreet(const QString &newStreet);

signals:

private:
    // 站点编号
    quint32 stationCode;

    // 站点名称
    QString stationName;

    // 经度
    QString longitude;

    // 纬度
    QString latitude;

    // 所在区域
    QString region;

    // 所在街道
    QString street;

};

#endif // STATION_H
