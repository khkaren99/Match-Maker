#include "matchMaker.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

#include <QMessageBox>

MatchMaker::MatchMaker(DataManager *_data, QObject *parent)
	: QObject(parent), data(_data)
{
	connect(this, &MatchMaker::addedWaitingUser, this, &MatchMaker::tryFindMatch);
}

void MatchMaker::requestMatch(const QString &userName)
{
	if (!waitList.contains(userName))
	{
		User *user = data->getUser(userName);
		if (user != nullptr)
		{
			assert(user->state == User::FREE);

			user->state = User::WAITING;
			waitList.insert(userName, user);
			emit addedWaitingUser(userName);
		}
	}
	else
	{
		QMessageBox::critical(nullptr, "Request Match", "User is in queue or already playing");
	}
}

void MatchMaker::freeUser(const QString &userName)
{
	if (waitList.contains(userName))
	{
		if (waitList[userName]->state != User::WAITING)
		{
			QMessageBox::critical(nullptr, "Free User", "User is playing could free");
			return;
		}
		waitList[userName]->state == User::FREE;
		waitList.remove(userName);
	}
}
#include <iostream>
void MatchMaker::runMatch(const QString &gameName, const QString &user1, const QString &user2)
{
	// Mark busy
	waitList[user1]->state = User::BUSY;
	waitList[user2]->state = User::BUSY;

	GameManager *game = new GameManager();
	std::cout << user1.toStdString() << " and " << user2.toStdString() << " start the " << gameName.toStdString() << std::endl;

	game->run(gameName, user1, user2);

	connect(game, &GameManager::gameResult, [&, gameName, user1, user2, game](const QString &winner)
			{
		std::cout << "The game finished. Winner: " << winner.toStdString() << std::endl;
		if (winner == user1)
			++waitList[user1]->preferredGame[gameName];
		if (winner == user2)
			++waitList[user2]->preferredGame[gameName];

		// remove from waitList
		waitList[user1]->state = User::FREE;
		waitList[user2]->state = User::FREE;
		waitList.remove(user1);
		waitList.remove(user2);

		emit data->dataUpdate();

		delete game; });
}

void MatchMaker::tryFindMatch(const QString &user1)
{
	for (auto user2 : waitList.keys())
	{
		if (user1 == user2)
			continue;

		// Look common games
		QString matchedGame;
		for (auto game : waitList[user1]->preferredGame.keys())
		{
			if (waitList[user2]->preferredGame.contains(game))
			{
				runMatch(game, user1, user2);
				return;
			}
		}
	}
}