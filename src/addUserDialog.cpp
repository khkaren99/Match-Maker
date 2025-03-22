#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVector>
#include <QString>
#include <QStringList>

#include "addUserDialog.h"
#include "common.h"

addUserDialog::addUserDialog(const QVector<QString>& games, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Add a User"));  

    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *userNameLine = new QHBoxLayout(this);
    QLabel *userNameLabel = new QLabel("Username:", this);
    userNameEdit = new QLineEdit(this);
    userNameLine->addWidget(userNameLabel);
    userNameLine->addWidget(userNameEdit);
    layout->addLayout(userNameLine, 0, 0);

    QHBoxLayout *firstNameLine = new QHBoxLayout(this);
    QLabel *firstNameLabel = new QLabel("First name:", this);
    firstNameEdit = new QLineEdit(this);
    firstNameLine->addWidget(firstNameLabel);
    firstNameLine->addWidget(firstNameEdit);
    layout->addLayout(firstNameLine, 1, 0);

    QHBoxLayout *lastNameLine = new QHBoxLayout(this);
    QLabel *lastNameLabel = new QLabel("Last name:", this);
    lastNameEdit = new QLineEdit(this);
    lastNameLine->addWidget(lastNameLabel);
    lastNameLine->addWidget(lastNameEdit);
    layout->addLayout(lastNameLine, 2, 0);

    QHBoxLayout *preferdGameLine = new QHBoxLayout(this);
    QLabel *preferdGameLabel = new QLabel("Preferd game:", this);
    preferdGameLine->addWidget(preferdGameLabel);
    for (auto game : games)
    {
        gameCheckBoxes.push_back(new QCheckBox(game, this));
        preferdGameLine->addWidget(gameCheckBoxes.back());
    }
    layout->addLayout(preferdGameLine, 3, 0);

    QHBoxLayout *buttonLine = new QHBoxLayout(this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    QPushButton *okButton = new QPushButton("OK", this);
    buttonLine->addWidget(cancelButton);
    buttonLine->addWidget(okButton);
    layout->addLayout(buttonLine, 4, 0, Qt::AlignRight);
    
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

QString addUserDialog::getUserName() const
{
    return userNameEdit->text();
}

QString addUserDialog::getFirstName() const
{
    return firstNameEdit->text();
}

QString addUserDialog::getLastName() const
{
    return lastNameEdit->text();
}

QStringList addUserDialog::getPreferdGames() const
{
    QStringList games;
    for (auto checkBox : gameCheckBoxes)
    {
        if (checkBox->isChecked())
            games.push_back(checkBox->text());
    }
    return games;
}