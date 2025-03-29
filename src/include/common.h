#pragma once

#include <QString>
#include <QStringList>

struct User
{
    enum UserState {FREE, BUSY, WAITING};

    QString userName;
    QString firstName;
    QString lastName;
    QStringList preferredGame;
    UserState state = FREE;
};
