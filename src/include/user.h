#pragma once

#include <QString>
#include <QHash>

struct User
{
    enum UserState
    {
        FREE,
        BUSY,
        WAITING
    };

    QString userName;
    QString firstName;
    QString lastName;
    QHash<QString, int> preferredGame;
    UserState state = FREE;
};
