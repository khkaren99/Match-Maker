#include "userList.h"
#include "addUserDialog.h"

#include <QLabel>
#include <QAction>
#include <QJsonArray>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSortFilterProxyModel>

UserList::UserList(DataManager *data, QWidget *parent)
    : QWidget(parent), m_users(data), m_tableView(new QTableView(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Creating tabel model and tabel view.
    // Set sorting and filtering proxy model.
    QAbstractTableModel *m_tableModel = new modeType(m_users, this);
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
    m_filterNameEdit = new QLineEdit(filterLine->widget());
    QPushButton *filterButton = new QPushButton("filter", filterLine->widget());
    filterLine->addWidget(filterNameLabel);
    filterLine->addWidget(m_filterNameEdit);
    filterLine->addWidget(filterButton);
    connect(filterButton, &QPushButton::clicked, this, &UserList::filter);

    // Collecting into main layout
    layout->addWidget(m_tableView, 0);
    layout->addLayout(filterLine, 1);

    setupContextMenu();
}

void UserList::addUser()
{
    addUserDialog::addUser(m_users);
}

void UserList::removeUser()
{
    // For remove need to be selected at least one row.
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
    QStringList userNames;
    for (auto index = selectedIndexes.begin(); index != selectedIndexes.end(); ++index)
        userNames.append(index->data().toString());

    for (auto user : userNames)
    {
        if (!m_users->removeUser(user))
        {
            QString message = "User " + user + " doesn't exists";
            QMessageBox::critical(this, "Remove a User", message);
            continue;
        }
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

void UserList::readUserList(QString fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open load file.");
        return;
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
        {
            QJsonObject gameInfo = game.toObject();
            user.preferredGame.insert(gameInfo["gameName"].toString(), gameInfo["gameRate"].toInt());
        }

        if (!m_users->addUser(user))
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
    auto data = m_users->getUsersList();
    for (auto user : data)
    {
        QJsonObject userInfo;
        userInfo["userName"] = user->userName;
        userInfo["firstName"] = user->firstName;
        userInfo["lastName"] = user->lastName;
        QJsonArray userGames;
        for (auto it = user->preferredGame.begin(); it != user->preferredGame.end(); ++it)
        {
            QJsonObject gameInfo;
            gameInfo["gameName"] = it.key();
            gameInfo["gameRate"] = it.value();
            userGames.append(gameInfo);
        }
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

    auto requestMatchAction = new QAction("Request Match For", m_tableView);
    connect(requestMatchAction, &QAction::triggered, [&]()
    {
        QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
        QStringList userNames;
        for (auto index = selectedIndexes.begin(); index != selectedIndexes.end(); ++index)
            emit requestMatch(index->data().toString());
    });
    m_tableView->addAction(requestMatchAction);

    auto freeMatchAction = new QAction("Free Matches For", m_tableView);
    connect(freeMatchAction, &QAction::triggered, [&]()
    {
        QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
        QStringList userNames;
        for (auto index = selectedIndexes.begin(); index != selectedIndexes.end(); ++index)
            emit freeMatch(index->data().toString());
    });
    m_tableView->addAction(freeMatchAction);

    m_tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void UserList::filter()
{
    auto proxyModel = static_cast<QSortFilterProxyModel *>(m_tableView->model());
    proxyModel->setFilterRegularExpression(QRegularExpression(m_filterNameEdit->text(),
                                                              QRegularExpression::CaseInsensitiveOption));
}
