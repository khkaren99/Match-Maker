#pragma once

#include "dataManager.h"

#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

class TreeModel : public QAbstractItemModel
{
private:
    struct Node
    {
        Node *parent = nullptr;
        QVector<Node *> children;
        QString gameName;
        User user;
    };

    QVector<Node *> tree;
    const QStringList header{"Game", "User", "Rating"};
    // this is need for fast access to exiting game,
    // and writing to file
    QHash<QString, Node *> games;

    DataManager *m_userData;

public:
    TreeModel(DataManager *users, QObject *parent = nullptr);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addUser(const User &user);
    void removeUser(const QString &userName);
};
