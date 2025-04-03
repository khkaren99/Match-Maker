#include "matchMaker.h"

#include <time.h>
#include <stdlib.h>

#include <QDebug>
#include <QMessageBox>

MatchMaker::MatchMaker(DataManager *data, QObject *parent)
	: QObject(parent)
	, m_data(data)
{
	connect(this, &MatchMaker::addedWaitingUser, this, &MatchMaker::tryFindMatch);
}

MatchMaker::~MatchMaker()
{
	// Wait for finishing all runing process
	while (m_games.size() != 0)
		m_games[0]->waitForFinished();
}

void MatchMaker::requestMatch(const QString &userName)
{
	if (!m_waitList.contains(userName))
	{
		std::shared_ptr<User> user = m_data->getUser(userName);
		if (user != nullptr)
		{
			assert(user->state == User::FREE);

			user->state = User::WAITING;
			m_waitList.insert(userName, user.get());
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
	if (m_waitList.contains(userName))
	{
		if (m_waitList[userName]->state != User::WAITING)
		{
			QMessageBox::critical(nullptr, "Free User", "User is playing could free");
			return;
		}
		m_waitList[userName]->state = User::FREE;
		m_waitList.remove(userName);
	}
}

void MatchMaker::printWaitList() const
{
	for (auto userName : m_waitList.keys())
	{
		qDebug() << userName;
	}
}

void MatchMaker::tryFindMatch(const QString &user1)
{
	srand(time(NULL));
	for (auto user2 : m_waitList.keys())
	{
		if (user1 == user2)
			continue;
		
		if (m_waitList[user2]->state == User::BUSY)
			continue;

		// Look common games
		QVector<QString> matchingGame;
		for (auto game : m_waitList[user1]->preferredGame.keys())
		{
			if (m_waitList[user2]->preferredGame.contains(game))
			{
				// check reating which is no greater than 3 or no lower than 1 of the requester’s rating
				int rateUser1 = m_waitList[user1]->preferredGame[game];
				int rateUser2 = m_waitList[user2]->preferredGame[game];
				if (qAbs(rateUser1 - rateUser2) <= 4)
					matchingGame.push_back(game);
			}
		}
		// run a rundom game
		if (matchingGame.size() > 0)
		{
			int gameCount = matchingGame.size();
			runMatch(matchingGame[rand() % gameCount], user1, user2);
			return;
		}
	}
}

void MatchMaker::runMatch(const QString &gameName, const QString &user1, const QString &user2)
{
	// Mark busy
	m_waitList[user1]->state = User::BUSY;
	m_waitList[user2]->state = User::BUSY;

	qDebug() << user1 << " and " << user2 << " start the " << gameName ;

	auto game = new GameManager();
	game->run(gameName, user1, user2);
	m_games.push_back(game);
	
	connect(game, &GameManager::gameResult, [&, gameName, user1, user2, game](const QString &winner)
	{
		qDebug() << "The game finished. Winner: " << winner;
		if (winner == user1)
			++m_waitList[user1]->preferredGame[gameName];
		if (winner == user2)
			++m_waitList[user2]->preferredGame[gameName];

		// remove from m_waitList
		m_waitList[user1]->state = User::FREE;
		m_waitList[user2]->state = User::FREE;
		m_waitList.remove(user1);
		m_waitList.remove(user2);

		emit m_data->dataUpdate();
		m_games.remove(m_games.indexOf(game));
		delete game;
	});
}
