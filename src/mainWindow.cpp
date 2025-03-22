#include <QAbstractItemModel>
#include <QMenuBar>

#include <iostream>

#include "mainWindow.h"
#include "dashboard.h"
#include "userList.h"


MainWindow::MainWindow()
{
    setFixedSize(1000, 300);
    m_splitter = new QSplitter(this);
    
    m_userList = new UserList(this);
    m_splitter->addWidget(m_userList);

    setCentralWidget(m_splitter);






    // Menu creation
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QAction *saveDashboardAct = new QAction(tr("Save the Dashboard to File"), this);
    fileMenu->addAction(saveDashboardAct);
//    connect(saveDashboardAct, &QAction::triggered, this, &MainWindow::saveDashboard);

    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));
    QAction *addUser = new QAction(tr("+Add User"), this);
    QAction *removeUser = new QAction(tr("-Remove User"), this);
    editMenu->addAction(addUser);
    editMenu->addAction(removeUser);
    connect(addUser, &QAction::triggered, m_userList, &UserList::addUser);
    connect(removeUser, &QAction::triggered, m_userList, &UserList::removeUser);

    QMenu *viewMenu = menuBar()->addMenu(tr("View"));
    QAction *shDashboard = new QAction(tr("Show/Hide Dashboard"), this);
    QAction *shUserList = new QAction(tr("Show/Hide User List"), this);
    viewMenu->addAction(shDashboard);
    viewMenu->addAction(shUserList);
//    connect(shDashboard, &QAction::triggered, this, &function);
//    connect(shUserList, &QAction::triggered, this, &function);





    
}