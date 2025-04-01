#pragma once

#include <QString>
#include <QHash>

struct User
{
    enum UserState
    {
        FREE = 0,
        BUSY,
        WAITING
    };

    QString userName;
    QString firstName;
    QString lastName;
    QHash<QString, int> preferredGame;
    UserState state = FREE;
};
