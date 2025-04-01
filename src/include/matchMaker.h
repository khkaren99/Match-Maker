#pragma once

#include "dataManager.h"
#include "gameManager.h"

#include <QObject>
#include <QString>

class MatchMaker : public QObject
{
	Q_OBJECT

private:
	DataManager *data;
	QHash<QString, User *> waitList;
	QList<GameManager *> runingGames;

public:
	MatchMaker(DataManager *data, QObject *parent = nullptr);

	void requestMatch(const QString &userName);
	void freeUser(const QString &userName);

private:
	void tryFindMatch(const QString &userName);
	void runMatch(const QString &game, const QString &user1, const QString &user2);

signals:
	void addedWaitingUser(const QString &userName);
};