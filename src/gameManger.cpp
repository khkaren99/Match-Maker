#include "gameManager.h"

#include <iostream>
#include <QCoreApplication>

GameManager::GameManager(QObject *parent)
	: QObject(parent)
	, m_process(std::make_unique<QProcess>())
{
	m_procPath = QCoreApplication::applicationDirPath() + "/game";

	connect(m_process.get(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), 
			this, &GameManager::applicationFinished);
}

void GameManager::run(const QString &game, const QString &user1, const QString &user2)
{
	QStringList args;
	args.append(game);
	args.append(user1);
	args.append(user2);

	m_process->start(m_procPath, args);
}

QProcess::ProcessState GameManager::state()
{
	return m_process->state();
}

bool GameManager::waitForFinished()
{
	return m_process->waitForFinished();
}

void GameManager::applicationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (exitStatus != QProcess::NormalExit)
	{
		std::cerr << "The game couldn't finish." << std::endl;
		return;
	}

	QString output = m_process->readAllStandardOutput();
	QStringList results = output.split("\n", Qt::SkipEmptyParts);
	QString winner = results.last().trimmed();

	emit gameResult(winner);
}