#include "tableModel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_users.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return headers.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QVariant(m_users[index.row()].userName);
        case 1:
            return QVariant(m_users[index.row()].firstName);
        case 2:
            return QVariant(m_users[index.row()].lastName);
        case 3:
            return QVariant(m_users[index.row()].preferdGame.join(", "));
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

bool TableModel::addUser(const User &user)
{
    // No double user name.
    // Check does the userName exist.
    for (const auto &u : m_users)
        if (u.userName == user.userName)
            return false;

    beginInsertRows(QModelIndex(), 0, 0);
    m_users.push_back(user);
    endInsertRows();
    return true;
}

bool TableModel::removeUser(const QString &userName)
{
    for (auto it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->userName == userName)
        {
            beginRemoveRows(QModelIndex(), 0, 0);
            m_users.erase(it);
            endRemoveRows();
            return true;
        }
    }
    return false;
}
