#pragma once

#include "dataManager.h"
#include "dashboard.h"
#include "userList.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
private:
    Dashboard *m_dashboard;
    UserList *m_userList;

public:
    MainWindow(DataManager *data);

private:
    void setupMenu();
};
