#pragma once

#include "common.h"
#include "treeModel.h"

#include <QWidget>
#include <QTreeView>
#include <QStringList>
#include <QAbstractItemModel>

class Dashboard : public QWidget
{
private:
    using modeType = TreeModel;

private:
    QAbstractItemModel *m_treeModel;
    QTreeView *m_treeView;

public:
    Dashboard(const QStringList &games, QWidget *parent = nullptr);

    void saveDashboard();
    void addUser(const User &user);
    void removeUser(const QString &userName);

private:
    void setupContextMenu();
};
