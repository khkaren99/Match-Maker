#include <QLabel>

#include "dashboard.h"

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    QLabel *label = new QLabel("Dashboard", this);
    label->setAlignment(Qt::AlignRight);
}

Dashboard::~Dashboard()
{
}

void Dashboard::saveDashboard()
{
    // Save the dashboard to a file
}
