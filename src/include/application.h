#pragma once

#include "dataManager.h"
#include "matchMaker.h"
#include "mainWindow.h"

#include <memory>

#include <QApplication>

class Application : public QApplication
{
	std::unique_ptr<DataManager> m_data;
	std::unique_ptr<MainWindow> m_mainWindow;
	std::unique_ptr<MatchMaker> m_core;

public:
	Application(int argc, char **argv);
	~Application();

	int exec();

private:
	void writeCache();
	void loadCache();
};
