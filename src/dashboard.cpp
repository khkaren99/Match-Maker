#include "dashboard.h"

#include <QFile>
#include <QAction>
#include <QJsonArray>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>

Dashboard::Dashboard(DataManager *_data, QWidget *parent)
    : QWidget(parent), data(_data), m_treeView(new QTreeView(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(this);

    QAbstractItemModel *m_treeModel = new modeType(data, this);

    m_treeView->setModel(m_treeModel);
    m_treeView->setFocusPolicy(Qt::NoFocus);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_treeView->setSelectionMode(QAbstractItemView::MultiSelection);

    layout->addWidget(m_treeView, 0);

    setupContextMenu();
}

void Dashboard::saveDashboard()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
    {
        // passes all game list to write into file
        writeDashboard(fileName, data->getGames());
    }
}

void Dashboard::writeDashboard(QString fileName, const QStringList &games)
{
    if (games.isEmpty())
        return;

    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    // array for each game in games
    QVector<QJsonArray> usersInfo(games.size());
    auto users = data->getUsers();
    // iterate over users and find fill the game data
    for (auto user : users)
    {
        // iterate over saving game list and check does
        // the user preferres the game
        for (size_t i = 0; i < games.size(); ++i)
        {
            if (user.preferredGame.contains(games[i]))
            {
                QJsonObject userInfo;
                userInfo["userName"] = user.userName;
                userInfo["rate"] = user.preferredGame[games[i]];
                usersInfo[i].append(userInfo);
            }
        }
    }
    QJsonObject dashboardData;
    for (size_t i = 0; i < usersInfo.size(); ++i)
    {
        dashboardData[games[i]] = usersInfo[i];
    }
    QJsonDocument saveDoc(dashboardData);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void Dashboard::setupContextMenu()
{
    auto addAction = new QAction("Save to file", this);
    connect(addAction, &QAction::triggered, [&]()
            {
        // get selected row and pass the selected rows gameNames for writing
        QModelIndexList selectedIndexes = m_treeView->selectionModel()->selectedRows();
        QStringList writingGames;
        for (auto index = selectedIndexes.begin(); index != selectedIndexes.end(); ++index)
        {
            QString gameName = index->data().toString();
            writingGames.push_back(gameName);
        }
        // make sure there is selected row
        if (writingGames.isEmpty())
            return;

        QString fileName = QFileDialog::getSaveFileName(this);
        if (!fileName.isEmpty())
            writeDashboard(fileName, writingGames); });
    this->addAction(addAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}
