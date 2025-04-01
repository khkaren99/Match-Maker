#include "dataManager.h"

#include <QMessageBox>

DataManager::DataManager(QObject *parent)
	: QObject(parent)
{
}

// return true if user added
bool DataManager::addUser(const User &user)
{
	if (m_users.contains(user.userName))
		return false;
	
	auto user_p = std::make_shared<User>(user);
	m_users.insert(user.userName, user_p);
	emit userAdded(m_users[user.userName]);
	return true;
}

// return true if user removed
bool DataManager::removeUser(const QString &userName)
{
	if (!m_users.contains(userName))
		return false;

	if (m_users[userName]->state != User::FREE)
	{
		QString message = userName + " could remove as it in Match process\n";
		message += "Free the User before remove";
		QMessageBox::critical(nullptr, "Remove User", message);
		return false;
	}

	m_users.remove(userName);
	emit userRemoved(userName);
	return true;
}

std::shared_ptr<User> DataManager::getUser(const QString &userName)
{
	if (m_users.contains(userName))
		return m_users[userName];
	return nullptr;
}

// return all users list
QList<std::shared_ptr<User>> DataManager::getUsersList() const
{
	return m_users.values();
}

size_t DataManager::usersCount() const
{
	return m_users.size();
}

bool DataManager::addGame(const QString &game)
{
	if (!m_games.contains(game))
	{
		m_games.append(game);
		return true;
	}
	return false;
}

bool DataManager::removeGame(const QString &game)
{
	if (m_games.contains(game))
	{
		m_games.removeAll(game);
		return true;
	}
	return false;
}

QStringList DataManager::getGames() const
{
	return m_games;
}
