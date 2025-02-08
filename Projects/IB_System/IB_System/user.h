#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDateTime>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);


    quint32 getCode() const;
    void setCode(quint32 newCode);

    QString getLoginName() const;
    void setLoginName(const QString &newLoginName);

    QString getPassword() const;
    void setPassword(const QString &newPassword);

    QString getName() const;
    void setName(const QString &newName);

    QString getPhone() const;
    void setPhone(const QString &newPhone);

    QString getIdCard() const;
    void setIdCard(const QString &newIdCard);

    quint8 getRole() const;
    void setRole(quint8 newRole);

    quint8 getDriving() const;
    void setDriving(quint8 newDriving);

    QString getStatus() const;
    void setStatus(const QString &newStatus);

    QString getRoleName() const;
    void setRoleName(const QString &newRoleName);

private:
    // 用户编号
    quint32 code;

    // 登录名
    QString loginName;

    // 密码
    QString password;

    // 名字
    QString name;

    // 电话
    QString phone;

    // 身份证号
    QString idCard;

    // 角色
    quint8 role;

    // 驾龄
    quint8 driving;

    // 状态
    QString status;

    // 角色名
    QString roleName;

signals:

};

#endif // USER_H
