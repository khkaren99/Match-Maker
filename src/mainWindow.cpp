#include "mainWindow.h"
#include "dashboard.h"
#include "userList.h"

#include <QMenuBar>

QStringList games = {"XO3", "Minesweeper", "RPS"};

MainWindow::MainWindow()
    : m_dashboard(new Dashboard(games, this))
    , m_userList(new UserList(games, this))
{
    setWindowTitle(tr("Matchmaking System"));
    resize(800, 600);

    connect(m_userList, &UserList::userAdded, m_dashboard, &Dashboard::addUser);
    connect(m_userList, &UserList::userRemoved, m_dashboard, &Dashboard::removeUser);

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
    QAction *saveDashboardAct = new QAction(tr("Save the Dashboard to File"), this);
    fileMenu->addAction(loadUsersAct);
    fileMenu->addAction(saveDashboardAct);
    connect(loadUsersAct, &QAction::triggered, m_userList, &UserList::loadUser);
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
