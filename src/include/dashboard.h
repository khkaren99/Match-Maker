#pragma once

#include <QWidget>

class Dashboard : public QWidget
{
public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void saveDashboard();
};