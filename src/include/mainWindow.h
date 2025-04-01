#pragma once

#include "dataManager.h"
#include "dashboard.h"
#include "userList.h"
#include "matchMaker.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
private:
    DataManager *m_data;
    Dashboard *m_dashboard;
    UserList *m_userList;
    MatchMaker *m_core;

public:
    MainWindow(DataManager *data);

    QString askUserName();

private:
    void setupMenu();
};
