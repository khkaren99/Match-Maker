#include "mainWindow.h"

#include <QLabel>
#include <QDialog>
#include <QMenuBar>
#include <QLineEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(DataManager *data, MatchMaker *core)
    : m_data(data)
    , m_dashboard(new Dashboard(data, this))
    , m_userList(new UserList(data, this))
    , m_core(core)
{
    setWindowTitle(tr("Matchmaking System"));
    resize(800, 600);

    QSplitter *spliter = new QSplitter(this);
    spliter->addWidget(m_dashboard);
    spliter->addWidget(m_userList);
    setCentralWidget(spliter);

    connect(m_userList, &UserList::requestMatch, m_core, &MatchMaker::requestMatch);
    connect(m_userList, &UserList::freeMatch, m_core, &MatchMaker::freeUser);

    setupMenu();
}

QString MainWindow::askUserName()
{
    // Create a dialog for imput userName.
    QDialog match;
    match.setWindowTitle("Make a match");
    QGridLayout *layout = new QGridLayout(&match);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *userNameLine = new QHBoxLayout(&match);
    QLabel *userNameLabel = new QLabel("User Name:", &match);
    QLineEdit *userNameEdit = new QLineEdit(&match);
    userNameLine->addWidget(userNameLabel);
    userNameLine->addWidget(userNameEdit);
    layout->addLayout(userNameLine, 0, 0);

    QHBoxLayout *buttonLine = new QHBoxLayout(&match);
    QPushButton *cancelButton = new QPushButton("Cancel", &match);
    QPushButton *okButton = new QPushButton("OK", &match);
    buttonLine->addWidget(cancelButton);
    buttonLine->addWidget(okButton);
    layout->addLayout(buttonLine, 4, 0, Qt::AlignRight);

    QObject::connect(okButton, &QPushButton::clicked, &match, &QDialog::accept);
    QObject::connect(cancelButton, &QPushButton::clicked, &match, &QDialog::reject);

    while (match.exec())
    {
        // Check that user is exist. This is not necessary as adds unnecessary
        // "depenance" MainWindow to DataManager but I preferred
        QString userName = userNameEdit->text();
        std::shared_ptr<User> user = m_data->getUser(userName);
        if (user.get() != nullptr)
            return userName;
        // else no user
        QMessageBox::critical(&match, "Make a match", "Undefined User");
    }
    return "";
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
    {
        m_dashboard->setVisible(!m_dashboard->isVisible());
    });
    connect(shUserList, &QAction::triggered, this, [&]()
    {
        m_userList->setVisible(!m_userList->isVisible());
    });

    QMenu *matchMakerMenu = menuBar()->addMenu(tr("Match Maker"));
    QAction *matchAct = new QAction(tr("Request Match"), this);
    QAction *freeAct = new QAction(tr("Free User"), this);
    QAction *printAct = new QAction(tr("Print User"), this);
    matchMakerMenu->addAction(matchAct);
    matchMakerMenu->addAction(freeAct);
    matchMakerMenu->addAction(printAct);
    connect(matchAct, &QAction::triggered, [&]()
    {
        QString userName = askUserName();
        m_core->requestMatch(userName);
    });
    connect(freeAct, &QAction::triggered, [&]()
    {
        QString userName = askUserName();
        m_core->freeUser(userName);
    });
    connect(printAct, &QAction::triggered, [&]()
    {
        m_core->printWaitList();
    });
}
