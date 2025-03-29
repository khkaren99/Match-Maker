#include "dashboard.h"

#include <QFile>
#include <QAction>
#include <QJsonArray>
#include <QFileDialog>
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
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectItems);

    layout->addWidget(m_treeView, 0);

    setupContextMenu();
    loadCache();
}

Dashboard::~Dashboard()
{
    writeCache();
}
void Dashboard::saveDashboard()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        writeDashboard(fileName);
}

void Dashboard::addUser(const User &user)
{
    static_cast<modeType *>(m_treeModel)->addUser(user.userName, user.preferredGame);
}

void Dashboard::removeUser(const QString &userName)
{
    static_cast<modeType *>(m_treeModel)->removeUser(userName);
}

void Dashboard::loadCache()
{
    QString fileName = ".mathcMaker_dashboard";
    if (QFile::exists(fileName))
        readDashboard(fileName);
}

void Dashboard::writeCache()
{
    QString fileName = ".mathcMaker_dashboard";
    writeDashboard(fileName);
}

void Dashboard::readDashboard(QString fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
    }

    QByteArray fileData = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(fileData));
    QJsonObject dashboardData = loadDoc.object();

    for (auto it = dashboardData.begin(); it != dashboardData.end(); ++it) {
        QString game = it.key();
        QJsonArray usersInfo = it.value().toArray();

        for (const QJsonValue &value : usersInfo) {
            QJsonObject userInfo = value.toObject();
            QString user = userInfo["user"].toString();
            QString rate = userInfo["rate"].toString();
            static_cast<modeType*>(m_treeModel)->addUser(user, game, rate);
        }
    }
}

void Dashboard::writeDashboard(QString fileName, const QStringList& games)
{
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject dashboardData;
    auto data = static_cast<modeType *>(m_treeModel)->treeDump();
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        // check does the game need to be saved
        if (!games.isEmpty() && !games.contains(it.key()))
            continue;

        QJsonArray usersInfo;
        for (auto user : it.value())
        {
            QJsonObject userInfo;
            userInfo["user"] = user.first;
            userInfo["rate"] = user.second;
            usersInfo.append(userInfo);
        }
        dashboardData[it.key()] = usersInfo;
    }
    QJsonDocument saveDoc(dashboardData);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void Dashboard::setupContextMenu()
{
    auto addAction = new QAction("Save to file", this);
    connect(addAction, &QAction::triggered, [&](){
        QModelIndexList selectedIndexes = m_treeView->selectionModel()->selectedRows();

        auto dataModel = static_cast<modeType *>(m_treeModel);
        QStringList writingGames;
        for (auto index = selectedIndexes.rbegin(); index != selectedIndexes.rend(); ++index)
        {
            QString gameName = index->data().toString();
            if (!gameName.isEmpty())
                writingGames.push_back(gameName);
        }
        if (writingGames.isEmpty())
            return;

        QString fileName = QFileDialog::getSaveFileName(this);
        if (!fileName.isEmpty())
            writeDashboard(fileName, writingGames);
    });
    this->addAction(addAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}
