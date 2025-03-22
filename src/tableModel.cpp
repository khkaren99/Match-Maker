#include "tableModel.h"

#include <QPixmap>
#include <iostream>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_users.push_back({"user1", "first2", "last3", {"XO3", "XO4"}});
    m_users.push_back({"user2", "first4", "last1", {"XO3", "XO5"}});
    m_users.push_back({"user3", "first1", "last5", {"XO3", "XO4", "XO5"}});
    m_users.push_back({"user4", "first5", "last2", {"XO4", "XO5"}});
    m_users.push_back({"user5", "first3", "last4", {"XO5"}});
}

TableModel::~TableModel()
{}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_users.size();//(parent.isValid() && parent.column() != 0) ? 0 : rc;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // {username, first name, last name, preferd game}
    return 4;
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
    // else if (orientation == Qt::Horizontal)
    QStringList headers = {"Username", "First name", "Last name", "Preferd games"};
    return headers[section];
}

bool TableModel::addUser(const User& user)
{
    for (const auto& u : m_users)
        if (u.userName == user.userName)
            return false;
    beginInsertRows(QModelIndex(), 0, 0);
        m_users.push_back(user);
    endInsertRows();
    return true;
}

bool TableModel::removeUser(const QString& userName)
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
