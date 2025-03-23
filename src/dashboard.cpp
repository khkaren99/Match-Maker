#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

#include "dashboard.h"
#include "treeModel.h"
#include "/home/khkaren/Documents/Examples/Qt-5.14.2/widgets/itemviews/interview/model.h"

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_treeModel = new TreeModel(this);

    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_treeModel);

    layout->addWidget(m_treeView, 0);
}

Dashboard::~Dashboard()
{
}

void Dashboard::saveDashboard()
{
    // Save the dashboard to a file
}
