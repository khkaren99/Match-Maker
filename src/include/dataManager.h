#pragma once

#include "user.h"

#include <memory>

#include <QHash>
#include <QObject>
#include <QStringList>

class DataManager : public QObject
{
	Q_OBJECT

private:
	QHash<QString, User> m_users;
	QStringList games;

public:
	DataManager(QObject *parent = nullptr);

	// return true if user added
	bool addUser(const User &user);
	// return true if user removed
	bool removeUser(const QString &userName);
	// fast access to user
	User *getUser(const QString &userName);

	// return all users list
	QList<User> getUsersList() const;
	size_t usersCount() const;

	bool addGame(const QString &gameName);
	bool removeGame(const QString &gameName);
	QStringList getGames() const;

signals:
	void dataUpdate();
	void userAdded(const User *user);
	void userRemoved(const QString &userName);
};
