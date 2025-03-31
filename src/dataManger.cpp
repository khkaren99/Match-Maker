#include "dataManager.h"

// return true if user added
bool DataManager::addUser(const User &user)
{
	if (m_users.contains(user.userName))
		return false;

	m_users.insert(user.userName, user);
	emit userAdded(user);
	return true;
}

// return true if user removed
bool DataManager::removeUser(const QString &userName)
{
	if (!m_users.contains(userName))
		return false;

	m_users.remove(userName);
	emit userRemoved(userName);
	return true;
}

// return all users list
QList<User> DataManager::getUsers()
{
	return m_users.values();
}

size_t DataManager::usersCount()
{
	return m_users.size();
}

bool DataManager::addGame(const QString &game)
{
	if (!games.contains(game))
	{
		games.append(game);
		return true;
	}
	return false;
}

bool DataManager::removeGame(const QString &game)
{
	if (games.contains(game))
	{
		games.removeAll(game);
		return true;
	}
	return false;
}

QStringList DataManager::getGames()
{
	return games;
}
