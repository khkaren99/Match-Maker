#pragma once

#include "dataManager.h"
#include "mainWindow.h"
#include "matchMaker.h"

#include <memory>

#include <QApplication>

class Application : public QApplication
{
	std::unique_ptr<DataManager> data;
	std::unique_ptr<MainWindow> mainWindow;

public:
	Application(int argc, char **argv);
	~Application();

	int exec();

private:
	void writeCache();
	void loadCache();
};
