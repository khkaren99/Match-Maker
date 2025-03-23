#include "treeModel.h"
#include <iostream>
#include <memory>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QString> games({"XO3", "XO4", "XO5"});
    int i = 0;
    for (auto game : games)
    {
        Node* node = new Node;
        node->data.push_back(game);
        node->level = 1;
        node->row = ++i;
        {
            Node* children = new Node;
            children->data.push_back("");
            children->data.push_back("user1");
            children->data.push_back("3");
            children->parent = node;
            children->level = 2;
            children->row = ++i;

            node->children.push_back(children);
            node->children.push_back(children);
        }

        tree.push_back(node);
    }
}

TreeModel::~TreeModel()
{
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &index) const
{
    Node* node = static_cast<Node*>(index.internalPointer());
    if (node && row < node->children.size() && column < node->children[row]->data.size())
        return createIndex(row, column, node->children[row]);
    
    if (!index.isValid() && row < tree.size() && column == 0)
        return createIndex(row, column, tree[row]);
    
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    Node* node = static_cast<Node*>(child.internalPointer());
    if (node && !tree.contains(node)) {
        Node* parentNode = node->parent;
        int row = tree.indexOf(parentNode);
        return createIndex(row, 0, parentNode);
    }
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Node* node = static_cast<Node*>(parent.internalPointer());
    if (node)
        return node->children.size();
    return tree.size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Node* node = static_cast<Node*>(parent.internalPointer());
    if (node)
        return node->data.size();
    return 3;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        Node* node = static_cast<Node*>(index.internalPointer());
        if (node->data.size() > index.column())
            return node->data[index.column()];
    }
    return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVector<QString> header({"Game", "User", "Rating"});
    if (role == Qt::DisplayRole)
        return header[section];
    return QVariant();
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    Node* node = static_cast<Node*>(parent.internalPointer());
    if (parent.isValid() && node)
        return !node->children.isEmpty();
    return !tree.empty();
}