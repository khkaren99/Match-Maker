#include "treeModel.h"

TreeModel::TreeModel(DataManager *data, QObject *parent)
    : QAbstractItemModel(parent)
    , m_userData(data)
{
    // Connecttion to data update
    connect(m_userData, &DataManager::userRemoved, [&](const QString &userName)
    {
        removeUser(userName);
    });
    connect(m_userData, &DataManager::userAdded, [&](std::shared_ptr<User> user)
    {
        addUser(user);
    });
    connect(m_userData, &DataManager::dataUpdate, [&]()
    {
        beginResetModel();
        endResetModel();
    });
}

// Valgring is clean in almost all scenarios.
TreeModel::~TreeModel()
{
    for (auto item : m_tree)
    {
        for (auto child : item->n_children)
            delete child;

        delete item;
    }
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // If parent isn't valid: means need root
    if (!parent.isValid())
    {
        // For root we need only 0 column
        if (row < m_tree.size() && column == 0)
            return createIndex(row, column, m_tree[row]);
        // else
        return QModelIndex();
    }
    // else the parent is valid and work with childs
    Node *node = static_cast<Node *>(parent.internalPointer());
    if (node != nullptr)
        return createIndex(row, column, node->n_children[row]);

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    // If the index don't contain pointer it's the root/(no parent).
    Node *node = static_cast<Node *>(child.internalPointer());
    if (node && !m_tree.contains(node))
    {
        Node *parentNode = node->n_parent;
        int row = m_tree.indexOf(parentNode);
        return createIndex(row, 0, parentNode);
    }
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Node *node = static_cast<Node *>(parent.internalPointer());
    if (node != nullptr)
        return node->n_children.size();
    return m_games.size();
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
        if (node->n_parent == nullptr)
        {
            return node->n_gameName;
        
        }
        else
        {
            // For child the column 0 should be empty
            switch (index.column())
            {
            case 1:
                return node->n_user->userName;
            case 2:
                return node->n_user->preferredGame[node->n_parent->n_gameName];
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

void TreeModel::addUser(std::shared_ptr<User>& user)
{
    for (auto game : user->preferredGame.keys())
    {
        // check if the game didn't add in m_tree them add it
        if (!m_games.contains(game))
        {
            Node *node = new Node;
            node->n_gameName = game;
            m_tree.push_back(node);
            m_games.insert(game, node);
        }
        // Check the user didn't add
        for (auto child : m_games[game]->n_children)
            if (child->n_user->userName == user->userName)
                return;

        // add a user in game
        Node *p = new Node;
        p->n_user = user;
        p->n_parent = m_games[game];

        beginInsertRows(QModelIndex(), 0, 0);
        m_games[game]->n_children.append(p);
        endInsertRows();
    }
}

void TreeModel::removeUser(const QString &userName)
{
    auto game = m_tree.begin();
    while (game != m_tree.end())
    {
        // remove user from game
        for (auto child : (*game)->n_children)
        {
            if (userName == child->n_user->userName)
            {
                beginRemoveRows(QModelIndex(), 0, 0);
                (*game)->n_children.remove((*game)->n_children.indexOf(child));
                endRemoveRows();
                delete child;
                break;
            }
        }
        // remove game if it's empty
        if ((*game)->n_children.isEmpty())
        {
            auto gameName = (*game)->n_gameName;
            beginRemoveRows(QModelIndex(), 0, 0);
            game = m_tree.erase(game);
            delete m_games[gameName];
            m_games.remove(gameName);
            endRemoveRows();
            continue;
        }

        ++game;
    }
}
