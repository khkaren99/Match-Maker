#pragma once

#include "dataManager.h"
#include "tableModel.h"

#include <QLineEdit>
#include <QTableView>

class UserList : public QWidget
{
    Q_OBJECT

private:
    using modeType = TableModel;

private:
    DataManager *m_users;
    QTableView *m_tableView;
    QLineEdit *m_filterNameEdit;

public:
    UserList(DataManager *data, QWidget *parent = nullptr);

    void addUser();
    void removeUser();

    void loadUserList();
    void saveUserList();

private:
    void readUserList(QString fileName);
    void writeUserList(QString fileName);

    void setupContextMenu();
    void filter();
};
