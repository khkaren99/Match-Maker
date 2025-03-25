#pragma once

#include "common.h"

#include <QVector>
#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
private:
    QVector<User> m_users;
    QStringList headers{"Username", "First name", "Last name", "Preferd games"};

public:
    TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool addUser(const User &user);
    bool removeUser(const QString &userName);

private:
    bool checkUser(const User &user);
};
