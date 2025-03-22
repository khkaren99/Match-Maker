#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QTableView>

#include "common.h"
#include "dashboard.h"
#include "userList.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSplitter* m_splitter;
    UserList* m_userList;
    Dashboard* m_dashboard;
public:
    MainWindow();

private:
    
};