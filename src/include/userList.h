#pragma once

#include "common.h"
#include "tableModel.h"
#include "addUserDialog.h"

#include <QTableView>

class UserList : public QWidget
{
    Q_OBJECT

private:
    using modeType = TableModel;

private:
    QAbstractTableModel *m_tableModel;
    QLineEdit *filterNameEdit;
    QTableView *m_tableView;
    QStringList games;

public:
    UserList(const QStringList &_games, QWidget *parent = nullptr);

    void addUser();
    void removeUser();

private:
    void setupContextMenu();
    void filter();
    bool validateUser(const User &user);

signals:
    void userAdded(const User &user);
    void userRemoved(const QString &userName);
};
