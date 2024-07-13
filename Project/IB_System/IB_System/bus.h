#ifndef BUS_H
#define BUS_H

#include <QObject>
#include <QDateTime>

class Bus : public QObject
{
    Q_OBJECT
public:
    explicit Bus(QObject *parent = nullptr);


    quint32 getBusCode() const;
    void setBusCode(quint32 newBusCode);

    QString getBusLicense() const;
    void setBusLicense(const QString &newBusLicense);

    QString getBusType() const;
    void setBusType(const QString &newBusType);

    QString getBusStatus() const;
    void setBusStatus(const QString &newBusStatus);

    QDateTime getStartTime() const;
    void setStartTime(const QDateTime &newStartTime);


private:
    // 车辆编号
    quint32 busCode;

    // 车牌号
    QString busLicense;

    // 车型
    QString busType;

    // 车辆状态
    QString busStatus;

    // 启用时间
    QDateTime startTime;

signals:

};

#endif // BUS_H
