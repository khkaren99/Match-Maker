#include "mainWindow.h"
#include "dashboard.h"
#include "userList.h"

#include <QMenuBar>
#include <QSplitter>

MainWindow::MainWindow(DataManager *data)
    : m_dashboard(new Dashboard(data, this)), m_userList(new UserList(data, this))
{
    setWindowTitle(tr("Matchmaking System"));
    resize(800, 600);

    QSplitter *spliter = new QSplitter(this);
    spliter->addWidget(m_dashboard);
    spliter->addWidget(m_userList);
    setCentralWidget(spliter);

    setupMenu();
}
void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QAction *loadUsersAct = new QAction(tr("Load users"), this);
    QAction *saveUsersAct = new QAction(tr("Save users"), this);
    QAction *saveDashboardAct = new QAction(tr("Save the Dashboard to File"), this);
    fileMenu->addAction(loadUsersAct);
    fileMenu->addAction(saveUsersAct);
    fileMenu->addAction(saveDashboardAct);
    connect(loadUsersAct, &QAction::triggered, m_userList, &UserList::loadUserList);
    connect(saveUsersAct, &QAction::triggered, m_userList, &UserList::saveUserList);
    connect(saveDashboardAct, &QAction::triggered, m_dashboard, &Dashboard::saveDashboard);

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
    connect(shDashboard, &QAction::triggered, this, [&]()
            { m_dashboard->setVisible(!m_dashboard->isVisible()); });
    connect(shUserList, &QAction::triggered, this, [&]()
            { m_userList->setVisible(!m_userList->isVisible()); });
}
