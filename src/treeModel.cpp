#include "treeModel.h"
#include <iostream>
#include <memory>

TreeModel::TreeModel(const QStringList &_games, QObject *parent)
    : QAbstractItemModel(parent)
    , games(_games)
{
    for (auto game : games)
    {
        Node *node = new Node;
        node->data.push_back(game);
        tree.push_back(node);
    }
}

// Didn't use smart pointer as QModelIndex use void* and
// there is same == checking. Also don't sure for ownership.
// Valgring is clean in almost all scenarios.
TreeModel::~TreeModel()
{
    for (auto item : tree)
    {
        for (auto child : item->children)
        {
            delete child;
        }
        delete item;
    }
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &index) const
{
    Node *node = static_cast<Node *>(index.internalPointer());
    if (node && row < node->children.size() &&
        column < node->children[row]->data.size())
        return createIndex(row, column, node->children[row]);

    if (!index.isValid() && row < tree.size() && column == 0)
        return createIndex(row, column, tree[row]);

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    // Assumption: if the index don't contain pointer it's the root/(no parent).
    Node *node = static_cast<Node *>(child.internalPointer());
    if (node && !tree.contains(node))
    {
        Node *parentNode = node->parent;
        int row = tree.indexOf(parentNode);
        return createIndex(row, 0, parentNode);
    }
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Node *node = static_cast<Node *>(parent.internalPointer());
    if (node)
        return node->children.size();
    return tree.size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Node *node = static_cast<Node *>(parent.internalPointer());
    if (node)
        return node->data.size();

    return header.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        Node *node = static_cast<Node *>(index.internalPointer());
        // if data.size() == 1 only for root.
        if (node->data.size() > index.column())
            return node->data[index.column()];
    }
    return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return header[section];
    return QVariant();
}

void TreeModel::addUser(const QString &userName, const QStringList &userGames)
{
    for (auto game : tree)
    {
        // Check the user didn't add
        for (auto child : game->children)
        {
            if (child->data[1] == userName)
                return;
        }
        if (userGames.contains(game->data[0].toString()))
        {
            // For ease of processing, pass an empty string as the first argument.
            // It was difficult to start the data from the second column.
            Node *p = new Node;
            p->data.push_back("");
            p->data.push_back(userName);
            p->data.push_back(0);
            p->parent = game;

            beginInsertRows(QModelIndex(), 0, 0);
            game->children.append(p);
            endInsertRows();
        }
    }
}

void TreeModel::removeUser(const QString &userName)
{
    for (auto game : tree)
    {
        for (auto user : game->children)
        {
            if (userName == user->data[1])
            {
                beginRemoveRows(QModelIndex(), 0, 0);
                game->children.remove(game->children.indexOf(user));
                endRemoveRows();
                delete user;
                break;
            }
        }
    }
}

void TreeModel::addUser(const QString &userName, const QString &game, const QString& rate)
{
    // get game tree
    int gameIndex = games.indexOf(game);
    // make sure game tree didn't contain the user
    for (auto child : tree[gameIndex]->children)
    {
        if (child->data[2] == userName)
            return;
    }

    // add user in game
    Node *p = new Node;
    p->data.push_back("");
    p->data.push_back(userName);
    p->data.push_back(rate);
    p->parent = tree[gameIndex];

    beginInsertRows(QModelIndex(), 0, 0);
    tree[gameIndex]->children.append(p);
    endInsertRows();
}

// No time for general ideas, as tree structure is fixed
TreeModel::DashboardData TreeModel::treeDump() const
{
    DashboardData data;

    for (auto it : tree)
    {
        // We know the tree keeps game, no need iterate on it data.
        // The game node has one data gameName.
        QString name = it->data[0].toString();
        data.insert(name, {});
        for (auto child : it->children)
        {
            // The data[0] is empty string, no need to dump.
            // The data[0] described in TreeModel::addUser().
            data[name].push_back({child->data[1].toString(), child->data[2].toString()});
        }
    }

    return data;
}
