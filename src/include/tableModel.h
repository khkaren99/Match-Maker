#pragma once

#include <QAbstractTableModel>
#include <QFileIconProvider>
#include <QStringList>
#include <QVector>

#include "common.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = nullptr);
    ~TableModel();

    
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool addUser(const User& user);
    bool removeUser(const QString& userName);

private:
    bool checkUser(const User& user);


    QVector<User> m_users;
};
