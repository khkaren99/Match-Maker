#include "treeModel.h"

TreeModel::TreeModel(DataManager *data, QObject *parent)
    : QAbstractItemModel(parent), m_userData(data)
{
    // Connecttion to data update
    connect(m_userData, &DataManager::userRemoved, [&](const QString &userName)
            { removeUser(userName); });
    connect(m_userData, &DataManager::userAdded, [&](const User &user)
            { addUser(user); });
}

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

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // If parent isn't valid: means need root
    if (!parent.isValid())
    {
        // For root we need only 0 column
        if (row < tree.size() && column == 0)
            return createIndex(row, column, tree[row]);
        // else
        return QModelIndex();
    }
    // else the parent is valid and work with childs
    Node *node = static_cast<Node *>(parent.internalPointer());
    if (node != nullptr)
        return createIndex(row, column, node->children[row]);

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    // If the index don't contain pointer it's the root/(no parent).
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
    return games.size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        Node *node = static_cast<Node *>(index.internalPointer());
        // if node doesn't has parent it is root
        if (node->parent == nullptr)
            return node->gameName;
        else
        {
            // For child the column 0 should be empty
            switch (index.column())
            {
            case 1:
                return node->user.userName;
                break;
            case 2:
                return node->user.preferredGame[node->parent->gameName];
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return header[section];
    return QVariant();
}

void TreeModel::addUser(const User &user)
{
    for (auto game : user.preferredGame.keys())
    {
        // check if the game didn't add in tree them add it
        if (!games.contains(game))
        {
            Node *node = new Node;
            node->gameName = game;
            tree.push_back(node);
            games.insert(game, node);
        }
        // Check the user didn't add
        for (auto child : games[game]->children)
            if (child->user.userName == user.userName)
                return;

        // add a user in game
        Node *p = new Node;
        p->user = user;
        p->parent = games[game];

        beginInsertRows(QModelIndex(), 0, 0);
        games[game]->children.append(p);
        endInsertRows();
    }
}

void TreeModel::removeUser(const QString &userName)
{
    auto game = tree.begin();
    while (game != tree.end())
    {
        // remove user from game
        for (auto child : (*game)->children)
        {
            if (userName == child->user.userName)
            {
                beginRemoveRows(QModelIndex(), 0, 0);
                (*game)->children.remove((*game)->children.indexOf(child));
                endRemoveRows();
                delete child;
                break;
            }
        }
        // remove game if it's empty
        if ((*game)->children.isEmpty())
        {
            auto gameName = (*game)->gameName;
            beginRemoveRows(QModelIndex(), 0, 0);
            game = tree.erase(game);
            games.remove(gameName);
            endRemoveRows();
            continue;
        }

        ++game;
    }
}
