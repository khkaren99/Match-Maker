#pragma once

#include "dataManager.h"

#include <QStringList>
#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
private:
    DataManager *m_users;
    const QStringList headers{"Username", "First name", "Last name", "Preferred games"};

public:
    TableModel(DataManager *users, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
