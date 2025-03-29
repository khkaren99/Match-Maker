#include "userList.h"
#include "addUserDialog.h"

#include <fstream>
#include <sstream>

#include <QLabel>
#include <QAction>
#include <QJsonArray>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSortFilterProxyModel>

UserList::UserList(const QStringList &_games, QWidget *parent)
    : QWidget(parent)
    , m_tableModel(new modeType(this))
    , m_tableView(new QTableView(this))
    , games(_games)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Creating tabel model and tabel view.
    // Set QSortFilterProxyModel for sorting and filtering.
    auto proxyModel = new QSortFilterProxyModel(m_tableModel);
    proxyModel->setSourceModel(m_tableModel);
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^[\\x20-\\x7E].*"),
                                                              QRegularExpression::CaseInsensitiveOption));

    m_tableView->setModel(proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setFocusPolicy(Qt::NoFocus);
    m_tableView->setSortingEnabled(true);
    m_tableView->sortByColumn(0, Qt::AscendingOrder);

    // Setup Filter line
    QHBoxLayout *filterLine = new QHBoxLayout(layout->widget());
    QLabel *filterNameLabel = new QLabel("UserName filter:", filterLine->widget());
    filterNameEdit = new QLineEdit(filterLine->widget());
    QPushButton *filterButton = new QPushButton("filter", filterLine->widget());
    filterLine->addWidget(filterNameLabel);
    filterLine->addWidget(filterNameEdit);
    filterLine->addWidget(filterButton);
    connect(filterButton, &QPushButton::clicked, this, &UserList::filter);

    // Collecting into main layout
    layout->addWidget(m_tableView, 0);
    layout->addLayout(filterLine, 1);

    setupContextMenu();
    loadCache();
}

UserList::~UserList()
{
    writeCache();
}

void UserList::addUser()
{
    addUserDialog dialog(games);
    // If the imput data is invalid ask to correct it.
    while (dialog.exec())
    {
        User user;
        user.userName = dialog.getUserName();
        user.firstName = dialog.getFirstName();
        user.lastName = dialog.getLastName();
        user.preferredGame = dialog.getPreferredGames();

        if (addUser_direct(user))
            break;
    }
}

void UserList::removeUser()
{
    // For remove need to be selected at least one row.
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
    auto dataModel = static_cast<modeType *>(m_tableModel);
    // Iterate from back as in the second iteration's index is going invalid.
    for (auto index = selectedIndexes.rbegin(); index != selectedIndexes.rend(); ++index)
    {
        QString userName = index->data().toString();
        if (!dataModel->removeUser(userName))
        {
            QString message = "User " + userName + " doesn't exists";
            QMessageBox::critical(this, "Remove a User", message);
            continue;
        }
        // Send the signal for dashboard.
        emit userRemoved(userName);
    }
}

void UserList::loadUserList()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (QFile::exists(fileName))
        readUserList(fileName);
}

void UserList::saveUserList()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        writeUserList(fileName);
}

void UserList::loadCache()
{
    QString fileName = ".mathcMaker_userList";
    if (QFile::exists(fileName))
        readUserList(fileName);
}

void UserList::writeCache()
{
    QString fileName = ".mathcMaker_userList";
    writeUserList(fileName);
}

void UserList::readUserList(QString fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
    }

    QByteArray fileData = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(fileData));
    QJsonArray userListData = loadDoc.array();

    for (auto it = userListData.begin(); it != userListData.end(); ++it)
    {
        QJsonObject userInfo = it->toObject();
        User user;
        user.userName = userInfo["userName"].toString();
        user.firstName = userInfo["firstName"].toString();
        user.lastName = userInfo["lastName"].toString();

        for (auto game : userInfo["preferredGame"].toArray())
            user.preferredGame.push_back(game.toString());
        
        if (!addUser_direct(user))
            qWarning("Invalid user");
    }
}

void UserList::writeUserList(QString fileName)
{
    // write a user list into file
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray userListData;
    auto data = static_cast<modeType*>(m_tableModel)->tableDump();
    for (auto item : data)
    {
        QJsonObject userInfo;
        userInfo["userName"] = item.toStringList()[0];
        userInfo["firstName"] = item.toStringList()[1];
        userInfo["lastName"] = item.toStringList()[2];
        QJsonArray userGames;
        for (auto game : item.toStringList()[3].split(' '))
            userGames.append(game);
        userInfo["preferredGame"] = userGames;

        userListData.append(userInfo);
    }
    QJsonDocument saveDoc(userListData);
    saveFile.write(saveDoc.toJson());
    
    saveFile.close();
}

void UserList::setupContextMenu()
{
    auto addAction = new QAction("+Add User", m_tableView);
    connect(addAction, &QAction::triggered, this, &UserList::addUser);
    m_tableView->addAction(addAction);

    auto removeAction = new QAction("-Remove User", m_tableView);
    connect(removeAction, &QAction::triggered, this, &UserList::removeUser);
    m_tableView->addAction(removeAction);

    m_tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void UserList::filter()
{
    auto proxyModel = static_cast<QSortFilterProxyModel *>(m_tableView->model());

    proxyModel->setFilterRegularExpression(QRegularExpression(filterNameEdit->text(),
                                                              QRegularExpression::CaseInsensitiveOption));
}

bool UserList::validateUser(const User &user)
{
    auto reportError = [&](const QString &message)
    {
        QMessageBox::critical(this, "Error Add a User", message);
    };

    QString userName = user.userName;
    if (userName.isEmpty() || userName.size() > 16 || userName.contains(" "))
    {
        reportError("Username must be between 1 and 16 characters and don't contain invisible characters");
        return false;
    }

    QString firstName = user.firstName;
    if (firstName.isEmpty() || !firstName.contains(QRegularExpression("^[A-Z]")) ||
        !firstName.contains(QRegularExpression("^[A-Za-z]+$")))
    {
        reportError("First name must consist of Latin characters only and starts with a capital letter.");
        return false;
    }

    QString lastName = user.lastName;
    if (lastName.isEmpty() || !lastName.contains(QRegularExpression("^[A-Z]")) ||
        !lastName.contains(QRegularExpression(R"(^[A-Za-z]+('?[A-Za-z]+)*$)")))
    {
        reportError("Last name must consist of Latin characters only and starts with a capital letter.");
        return false;
    }

    QStringList preferredGames = user.preferredGame;
    if (preferredGames.isEmpty())
    {
        reportError("You must select at least one game");
        return false;
    }

    return true;
}

bool UserList::addUser_direct(const User& user)
{
    if (!validateUser(user))
        return false;

    auto dataModel = static_cast<modeType *>(m_tableModel);
    if (!dataModel->addUser(user))
    {
        QString message = "User " + user.userName + " already exists";
        QMessageBox::critical(this, "Error Add a User", message);
        return false;
    }
    // Send the signal for dashboard.
    emit userAdded(user);
    return true;
}
