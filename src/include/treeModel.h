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
        Node *n_parent = nullptr;
        QVector<Node *> n_children;
        QString n_gameName;
        std::shared_ptr<User> n_user;
    };

    const QStringList header{"Game", "User", "Rating"};
    QVector<Node *> m_tree;
    DataManager *m_userData;
    // this is need for fast access to exiting game,
    // and writing to file
    QHash<QString, Node *> m_games;

public:
    TreeModel(DataManager *users, QObject *parent = nullptr);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addUser(std::shared_ptr<User>& user);
    void removeUser(const QString &userName);
};
