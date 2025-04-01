#pragma once

#include "treeModel.h"
#include "dataManager.h"

#include <QWidget>
#include <QTreeView>
#include <QStringList>

class Dashboard : public QWidget
{
private:
    using modeType = TreeModel;

private:
    DataManager *m_data;
    QTreeView *m_treeView;

public:
    Dashboard(DataManager *data, QWidget *parent = nullptr);

    void saveDashboard();

private:
    // games specify which games data should be writen.The argument
    // added to avoid code dupication.
    void writeDashboard(QString fileName, const QStringList &games = {});

    void setupContextMenu();
};
