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
    ~UserList();

    void addUser();
    void removeUser();

    void loadUserList();
    void saveUserList();
    
private:
    void loadCache(); 
    void writeCache();
    void readUserList(QString fileName);
    void writeUserList(QString fileName);

    void setupContextMenu();
    void filter();
    bool validateUser(const User &user);
    bool addUser_direct(const User& user);

signals:
    void userAdded(const User &user);
    void userRemoved(const QString &userName);
};
