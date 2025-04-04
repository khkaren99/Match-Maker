#include "application.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
{
    m_data.reset(new DataManager);
    m_core.reset(new MatchMaker(m_data.get()));
    m_mainWindow.reset(new MainWindow(m_data.get(), m_core.get()));

    loadCache();
}

Application::~Application()
{
    writeCache();
}

int Application::exec()
{
    QStringList games = {"XO3", "RPS", "Minesweeper"};
    for (auto game : games)
        m_data->addGame(game);

    m_mainWindow->show();

    return QApplication::exec();
}

// This function muches with UserList::writeUserList
void Application::writeCache()
{
    QString fileName = ".matchMaker_data";
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray userListData;
    auto users = m_data->getUsersList();
    for (auto user : users)
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

// This function muches with UserList::readUserList
void Application::loadCache()
{
    QString fileName = ".matchMaker_data";
    if (!QFile::exists(fileName))
        return;

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

        if (!m_data->addUser(user))
            qWarning("Invalid user");
    }
}
