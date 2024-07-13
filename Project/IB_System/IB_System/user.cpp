#include "user.h"

User::User(QObject *parent)
    : QObject{parent}
{

}

quint32 User::getCode() const
{
    return code;
}

void User::setCode(quint32 newCode)
{
    code = newCode;
}

QString User::getLoginName() const
{
    return loginName;
}

void User::setLoginName(const QString &newLoginName)
{
    loginName = newLoginName;
}

QString User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &newPassword)
{
    password = newPassword;
}

QString User::getName() const
{
    return name;
}

void User::setName(const QString &newName)
{
    name = newName;
}

QString User::getPhone() const
{
    return phone;
}

void User::setPhone(const QString &newPhone)
{
    phone = newPhone;
}

QString User::getIdCard() const
{
    return idCard;
}

void User::setIdCard(const QString &newIdCard)
{
    idCard = newIdCard;
}

quint8 User::getRole() const
{
    return role;
}

void User::setRole(quint8 newRole)
{
    role = newRole;
}

quint8 User::getDriving() const
{
    return driving;
}

void User::setDriving(quint8 newDriving)
{
    driving = newDriving;
}

QString User::getStatus() const
{
    return status;
}

void User::setStatus(const QString &newStatus)
{
    status = newStatus;
}

QString User::getRoleName() const
{
    return roleName;
}

void User::setRoleName(const QString &newRoleName)
{
    roleName = newRoleName;
}
