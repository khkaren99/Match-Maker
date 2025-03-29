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
    ~Dashboard();


    void saveDashboard();
    void addUser(const User &user);
    void removeUser(const QString &userName);

private:
    // TODO: need to syncronize data with userList to avoid
    //       userName in games tree who not in user list
    //       after can add loadDashboard from custom file.
    //       Now reading as cache file created from valid data
    void loadCache();
    void writeCache();
    void readDashboard(QString fileName);
    void writeDashboard(QString fileName);

    void setupContextMenu();
};
