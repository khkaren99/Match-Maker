#pragma once

#include <QObject>
#include <QString>
#include <QProcess>

class GameManager : public QObject
{
	Q_OBJECT

private:
	QProcess *process;
	QString procPath;

public:
	GameManager(QObject *parent = nullptr);

	QString getGameName();
	void run(const QString &game, const QString &user1, const QString &user2);

	QProcess::ProcessState state();
	bool waitForFinished();

private:
	void applicationFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
	void gameResult(QString &winner);
};