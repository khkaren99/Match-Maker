#pragma once

#include <QString>
#include <QHash>

struct User
{
    QString userName;
    QString firstName;
    QString lastName;
    QHash<QString, unsigned int> preferdGame;
};