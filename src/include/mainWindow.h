#pragma once

#include "dashboard.h"
#include "userList.h"

#include <QWidget>
#include <QSplitter>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
private:
    Dashboard *m_dashboard;
    UserList *m_userList;

public:
    MainWindow();

private:
    void setupMenu();
};
