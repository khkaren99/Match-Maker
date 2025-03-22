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
private:
    QAbstractTableModel* m_tableModel;
    QTableView* m_tableView;

public:
    UserList(QWidget *parent);
    ~UserList();

    void addUser();
    void removeUser();
};