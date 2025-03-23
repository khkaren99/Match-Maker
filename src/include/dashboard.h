#pragma once

#include <QWidget>
#include <QAbstractItemModel>
#include <QTreeView>

class Dashboard : public QWidget
{
    Q_OBJECT
private:
    QAbstractItemModel* m_treeModel;
    QTreeView *m_treeView;

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void saveDashboard();
};