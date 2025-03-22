#include "userList.h"
#include "tableModel.h"
#include "addUserDialog.h"

#include <iostream>

#include <QMessageBox>
#include <QTableView>
#include <QAbstractTableModel>
#include <QModelIndexList>
#include <QSortFilterProxyModel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QtWidgets>


UserList::UserList(QWidget *parent)
: QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_tableModel = new TableModel(this);
    
    auto proxyModel = new QSortFilterProxyModel(m_tableModel);
    proxyModel->setSourceModel(m_tableModel);
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^[A-Z].*"),
        QRegularExpression::CaseInsensitiveOption));    
    
    m_tableView = new QTableView(layout->widget());
    m_tableView->setModel(proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setFocusPolicy(Qt::NoFocus);
    m_tableView->setSortingEnabled(true);

    QHBoxLayout *filterLine = new QHBoxLayout(layout->widget());
    QLabel *filterNameLabel = new QLabel("UserName filter:", filterLine->widget());
    auto filterNameEdit = new QLineEdit(filterLine->widget());
    QPushButton *filterButton = new QPushButton("filter", filterLine->widget());
    filterLine->addWidget(filterNameLabel);
    filterLine->addWidget(filterNameEdit);
    filterLine->addWidget(filterButton);
    
    layout->addWidget(m_tableView, 0);
    layout->addLayout(filterLine, 1);
    connect(filterButton, &QPushButton::clicked, this, &UserList::filter);
}

UserList::~UserList()
{
    std::cout << "called" << std::endl;
}

void UserList::addUser()
{
    addUserDialog dialog({"XO3", "XO4", "XO5"});
    while (dialog.exec())
    {
        // Check correctness of the input
        {
            QString userName = dialog.getUserName();
            if (userName.isEmpty() || userName.size() > 16 || userName.contains(" "))
            {
                QMessageBox::critical(this, "Error Add a User", 
                    "Username must be between 1 and 16 characters and don't contain invisible characters");
                continue;
            }
            
            QString firstName = dialog.getFirstName();
            if (firstName.isEmpty() || !firstName.contains(QRegularExpression("^[A-Z]")) || 
                                       !firstName.contains(QRegularExpression("^[A-Za-z]+$")))
            {
                QMessageBox::critical(this, "Error Add a User", 
                    "First name must consist of Latin characters only and starts with a capital letter.");
                continue;
            }

            QString lastName = dialog.getLastName();
            if (lastName.isEmpty() || !lastName.contains(QRegularExpression("^[A-Z]")) || 
                                      !lastName.contains(QRegularExpression(R"(^[A-Za-z]+('?[A-Za-z]+)*$)")))
            {
                QMessageBox::critical(this, "Error Add a User", 
                    "Last name must consist of Latin characters only and starts with a capital letter.");
                continue;
            }

            QStringList preferdGames = dialog.getPreferdGames();
            if (preferdGames.isEmpty())
            {
                QMessageBox::critical(this, "Error Add a User", "You must select at least one game");
                continue;
            }
        }

        User user;
        user.userName = dialog.getUserName();
        user.firstName = dialog.getFirstName();
        user.lastName = dialog.getLastName();
        user.preferdGame = dialog.getPreferdGames();


        auto dataModel = dynamic_cast<TableModel*>(m_tableModel);
        if (!dataModel->addUser(user))
        {
            QString message = "User " + user.userName + " already exists";
            QMessageBox::critical(this, "Error Add a User", message);
            continue;
        }
        emit userAdded(user.userName);
        break;
    }
}

void UserList::removeUser()
{
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
    auto dataModel = static_cast<TableModel*>(m_tableModel);
    for (const QModelIndex &index : selectedIndexes) {
        QModelIndex parent = index.parent();
        QVariant userName = m_tableModel->index(index.row(), 0 /*0 column is userName*/, parent).data();

        if (!dataModel->removeUser(userName.toString()))
        {
            QString message = "User " + userName.toString() + " doesn't exists";
            QMessageBox::critical(this, "Remove a User", message);
            continue;
        }
        emit userRemoved(userName.toString());
    }
}

void UserList::filter()
{
    auto proxyModel = dynamic_cast<QSortFilterProxyModel*>(m_tableView->model());
    if (!proxyModel)
    {
        return;
    }

    auto filterEdit = findChild<QLineEdit*>();
    if (!filterEdit)
    {
        return;
    }

    proxyModel->setFilterRegularExpression(QRegularExpression(filterEdit->text(),
        QRegularExpression::CaseInsensitiveOption));
}