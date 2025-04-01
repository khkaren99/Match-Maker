#pragma once

#include <memory>

#include <QObject>
#include <QString>
#include <QProcess>

class GameManager : public QObject
{
	Q_OBJECT

private:
	std::unique_ptr<QProcess> m_process;
	QString m_procPath;

public:
	GameManager(QObject *parent = nullptr);

	void run(const QString &game, const QString &user1, const QString &user2);

	QProcess::ProcessState state();
	bool waitForFinished();

private:
	void applicationFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
	void gameResult(QString &winner);
};