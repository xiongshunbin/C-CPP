#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QDateTime>

class Line : public QObject
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);


    quint32 getLineCode() const;
    void setLineCode(quint32 newLineCode);

    QString getLineName() const;
    void setLineName(const QString &newLineName);

    QString getStatus() const;
    void setStatus(const QString &newStatus);

    QDateTime getStartLineTime() const;
    void setStartLineTime(const QDateTime &newStartLineTime);

    QString getDirection() const;
    void setDirection(const QString &newDirection);

    QList<QString> getAllStation() const;
    void setAllStation(const QList<QString> &newAllStation);

private:
    // 线路编号
    quint32 lineCode;

    // 线路名称
    QString lineName;

    // 状态
    QString status;

    // 开线时间
    QDateTime startLineTime;

    // 方向
    QString direction;

    // 包含站点
    QList<QString> allStation;

signals:

};

#endif // LINE_H
