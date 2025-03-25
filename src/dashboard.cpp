#include "dashboard.h"

#include <QFile>
#include <QAction>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>

Dashboard::Dashboard(const QStringList &games, QWidget *parent)
    : QWidget(parent)
    , m_treeModel(new modeType(games, this))
    , m_treeView(new QTreeView(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_treeView->setModel(m_treeModel);
    m_treeView->setFocusPolicy(Qt::NoFocus);
    m_treeView->setSelectionMode(QAbstractItemView::NoSelection);

    layout->addWidget(m_treeView, 0);
    setupContextMenu();
}

void Dashboard::saveDashboard()
{
    QFile saveFile(QStringLiteral("dashboard.json"));
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject gameInfo;
    auto data = static_cast<modeType *>(m_treeModel)->treeDump();
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        QJsonArray usersInfo;
        for (auto user : it.value())
        {
            QJsonObject userInfo;
            userInfo["user"] = user.first;
            userInfo["rate"] = user.second;
            usersInfo.append(userInfo);
        }
        gameInfo[it.key()] = usersInfo;
    }
    QJsonDocument saveDoc(gameInfo);
    saveFile.write(saveDoc.toJson());
}

void Dashboard::addUser(User user)
{
    static_cast<modeType *>(m_treeModel)->addUser(user.userName, user.preferdGame);
}

void Dashboard::removeUser(const QString &userName)
{
    static_cast<modeType *>(m_treeModel)->removeUser(userName);
}

void Dashboard::setupContextMenu()
{
    auto addAction = new QAction("Save to file", this);
    connect(addAction, &QAction::triggered, this, &Dashboard::saveDashboard);
    this->addAction(addAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}
