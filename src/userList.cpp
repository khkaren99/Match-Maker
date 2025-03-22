#include "userList.h"
#include "tableModel.h"
#include "addUserDialog.h"

#include <iostream>

#include <QMessageBox>
#include <QTableView>
#include <QAbstractTableModel>
#include <QModelIndexList>
#include <QSortFilterProxyModel>

UserList::UserList(QWidget *parent)
    : QWidget(parent)
{
    m_tableView = new QTableView(this);
    m_tableModel = new TableModel(m_tableView);

    auto proxyModel = new QSortFilterProxyModel(m_tableModel);
    proxyModel->setSourceModel(m_tableModel);
    proxyModel->setFilterRegularExpression(QRegularExpression(QString("^[A-Z].*"),
        QRegularExpression::CaseInsensitiveOption));
    
    
    m_tableView->setModel(proxyModel);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setFocusPolicy(Qt::NoFocus);
    m_tableView->setSortingEnabled(true);


//    m_tableView->resize(500, 300);

//    resize(500, 300);
//    tableView->resize(500, 300);
}

UserList::~UserList()
{
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
        break;
    }
}

void UserList::removeUser()
{
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
    auto dataModel = static_cast<TableModel*>(m_tableModel);
    for (const QModelIndex &index : selectedIndexes) {
        QModelIndex parent = index.parent();
        QVariant data = m_tableModel->index(index.row(), 0, parent).data(0);

        if (!dataModel->removeUser(data.toString()))
        {
            QString message = "User " + data.toString() + " doesn't exists";
            QMessageBox::critical(this, "Remove a User", message);
        }
    }
}
