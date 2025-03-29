#pragma once

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
        QVector<QVariant> data;
    };

    QVector<Node *> tree;
    const QStringList games;
    QStringList header{"Game", "User", "Rating"};

public:
    explicit TreeModel(const QStringList &_games, QObject *parent = nullptr);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addUser(const QString &userName, const QStringList &games);
    void addUser(const QString &userName, const QString &game, const QString& rate);
    void removeUser(const QString &userName);

    // using another section as there is some specific usings
    // gameName
    //     |
    //     +--- | UserName | raite |
    // gameName maps to the vector of {username, rate} pairs.
private:
    using UserInfo = QPair<QString, QString>;
    using DashboardData = QHash<QString, QVector<UserInfo>>;

public:
    DashboardData treeDump() const;
};
