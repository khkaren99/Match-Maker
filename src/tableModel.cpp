#include "tableModel.h"

TableModel::TableModel(DataManager *users, QObject *parent)
    : QAbstractTableModel(parent), m_users(users)
{
    // Connecttion to data updates
    connect(m_users, &DataManager::userAdded, [&](const User *)
            {
            beginResetModel();
            endResetModel(); });
    connect(m_users, &DataManager::userRemoved, [&](const QString &)
            {
            beginResetModel();
            endResetModel(); });
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_users->usersCount();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return headers.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        auto users = m_users->getUsersList();
        switch (index.column())
        {
        case 0:
            return QVariant(users[index.row()].userName);
        case 1:
            return QVariant(users[index.row()].firstName);
        case 2:
            return QVariant(users[index.row()].lastName);
        case 3:
            return QVariant(users[index.row()].preferredGame.keys().join(", "));
        }
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return QString::number(section);

    return headers[section];
}
