#pragma once

#include "user.h"

#include <QHash>
#include <QObject>
#include <QStringList>

class DataManager : public QObject
{
	Q_OBJECT

public:
	// return true if user added
	bool addUser(const User &user);
	// return true if user removed
	bool removeUser(const QString &userName);
	// return all users list
	QList<User> getUsers();
	size_t usersCount();

	bool addGame(const QString &gameName);
	bool removeGame(const QString &gameName);
	QStringList getGames();

private:
	QHash<QString, User> m_users;
	QStringList games;

signals:
	void userAdded(const User &user);
	void userRemoved(const QString &userName);
};
