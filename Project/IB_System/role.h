#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include <QDateTime>

class Role : public QObject
{
    Q_OBJECT
public:
    explicit Role(QObject *parent = nullptr);


    quint32 getRoleCode() const;
    void setRoleCode(quint32 newRoleCode);

    QString getRoleName() const;
    void setRoleName(const QString &newRoleName);

    QString getRoledescribe() const;
    void setRoledescribe(const QString &newRoledescribe);

private:
    quint32 roleCode;
    QString roleName;
    QString roledescribe;

signals:

};

#endif // ROLE_H
