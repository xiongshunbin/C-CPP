#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <QObject>

class Scheduling : public QObject
{
    Q_OBJECT
public:
    explicit Scheduling(QObject *parent = nullptr);

    quint32 getCode() const;
    void setCode(quint32 newCode);

    quint32 getLineCode() const;
    void setLineCode(quint32 newLineCode);

    QString getTcNumber() const;
    void setTcNumber(const QString &newTcNumber);

    QString getTcTime() const;
    void setTcTime(const QString &newTcTime);

    quint32 getUserCode() const;
    void setUserCode(quint32 newUserCode);

    QString getStartStation() const;
    void setStartStation(const QString &newStartStation);

    QString getEndStation() const;
    void setEndStation(const QString &newEndStation);

    quint32 getBusCode() const;
    void setBusCode(quint32 newBusCode);

    QString getLineName() const;
    void setLineName(const QString &newLineName);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    QString getBusLicense() const;
    void setBusLicense(const QString &newBusLicense);

signals:


private:
    quint32 code;
    quint32 lineCode;
    QString tcNumber;
    QString tcTime;
    quint32 userCode;
    QString startStation;
    QString endStation;
    quint32 busCode;
    QString lineName;
    QString userName;
    QString busLicense;
};

#endif // SCHEDULING_H
