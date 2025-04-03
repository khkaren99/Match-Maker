#pragma once

#include "user.h"
#include "dataManager.h"
#include "gameManager.h"

#include <memory>

#include <QHash>
#include <QObject>
#include <QString>

class MatchMaker : public QObject
{
	Q_OBJECT

private:
	DataManager *m_data;
	QHash<QString, User *> m_waitList;
	QVector<GameManager*> m_games;

public:
	MatchMaker(DataManager *data, QObject *parent = nullptr);
	~MatchMaker();

	void requestMatch(const QString &userName);
	void freeUser(const QString &userName);

	void printWaitList() const;

private:
	void tryFindMatch(const QString &userName);
	void runMatch(const QString &game, const QString &user1, const QString &user2);

signals:
	void addedWaitingUser(const QString &userName);
};