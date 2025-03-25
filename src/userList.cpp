#include "userList.h"
#include "addUserDialog.h"

#include <QLabel>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
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
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^[A-Z].*"),
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
        user.preferdGame = dialog.getPreferdGames();

        if (!validateUser(user))
            continue;

        auto dataModel = static_cast<modeType *>(m_tableModel);
        if (!dataModel->addUser(user))
        {
            QString message = "User " + user.userName + " already exists";
            QMessageBox::critical(this, "Error Add a User", message);
            continue;
        }
        // Send the signal for dashboard.
        emit userAdded(user);
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
        QVariant userName = index->data();
        if (!dataModel->removeUser(userName.toString()))
        {
            QString message = "User " + userName.toString() + " doesn't exists";
            QMessageBox::critical(this, "Remove a User", message);
            continue;
        }
        // Send the signal for dashboard.
        emit userRemoved(userName.toString());
    }
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

    QStringList preferdGames = user.preferdGame;
    if (preferdGames.isEmpty())
    {
        reportError("You must select at least one game");
        return false;
    }

    return true;
}
