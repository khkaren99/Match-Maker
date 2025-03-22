#pragma once

#include <QWidget>
#include <QVector>
#include <QTableView>
#include <QAbstractTableModel>

#include <unordered_map>
#include "common.h"
#include "addUserDialog.h"

class UserList : public QWidget
{
    Q_OBJECT
private:
    QAbstractTableModel* m_tableModel;
    QTableView* m_tableView;

public:
    UserList(QWidget *parent = nullptr);
    ~UserList();

    void addUser();
    void removeUser();

signals:
    void userAdded(const QString& userName);
    void userRemoved(const QString& userName);

private:
    void filter();
};