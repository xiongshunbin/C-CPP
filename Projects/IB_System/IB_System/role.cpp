#include "role.h"

Role::Role(QObject *parent)
    : QObject{parent}
{

}

quint32 Role::getRoleCode() const
{
    return roleCode;
}

void Role::setRoleCode(quint32 newRoleCode)
{
    roleCode = newRoleCode;
}

QString Role::getRoleName() const
{
    return roleName;
}

void Role::setRoleName(const QString &newRoleName)
{
    roleName = newRoleName;
}

QString Role::getRoledescribe() const
{
    return roledescribe;
}

void Role::setRoledescribe(const QString &newRoledescribe)
{
    roledescribe = newRoledescribe;
}
