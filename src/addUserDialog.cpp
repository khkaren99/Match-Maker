#include "user.h"
#include "addUserDialog.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>

addUserDialog::addUserDialog(const QStringList &games, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Add a User"));

    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *userNameLine = new QHBoxLayout(this);
    QLabel *userNameLabel = new QLabel("Username:", this);
    m_userNameEdit = new QLineEdit(this);
    userNameLine->addWidget(userNameLabel);
    userNameLine->addWidget(m_userNameEdit);
    layout->addLayout(userNameLine, 0, 0);

    QHBoxLayout *firstNameLine = new QHBoxLayout(this);
    QLabel *firstNameLabel = new QLabel("First name:", this);
    m_firstNameEdit = new QLineEdit(this);
    firstNameLine->addWidget(firstNameLabel);
    firstNameLine->addWidget(m_firstNameEdit);
    layout->addLayout(firstNameLine, 1, 0);

    QHBoxLayout *lastNameLine = new QHBoxLayout(this);
    QLabel *lastNameLabel = new QLabel("Last name:", this);
    m_lastNameEdit = new QLineEdit(this);
    lastNameLine->addWidget(lastNameLabel);
    lastNameLine->addWidget(m_lastNameEdit);
    layout->addLayout(lastNameLine, 2, 0);

    QHBoxLayout *preferredGameLine = new QHBoxLayout(this);
    QLabel *preferredGameLabel = new QLabel("Preferred game:", this);
    preferredGameLine->addWidget(preferredGameLabel);
    for (auto game : games)
    {
        m_gameCheckBoxes.push_back(new QCheckBox(game, this));
        preferredGameLine->addWidget(m_gameCheckBoxes.back());
    }
    layout->addLayout(preferredGameLine, 3, 0);

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
    return m_userNameEdit->text();
}

QString addUserDialog::getFirstName() const
{
    return m_firstNameEdit->text();
}

QString addUserDialog::getLastName() const
{
    return m_lastNameEdit->text();
}

QStringList addUserDialog::getPreferredGames() const
{
    QStringList games;
    for (auto checkBox : m_gameCheckBoxes)
    {
        if (checkBox->isChecked())
            games.push_back(checkBox->text());
    }
    return games;
}

void addUserDialog::addUser(DataManager *data)
{
    addUserDialog dialog(data->getGames());
    // If the imput data is invalid ask to correct it.
    while (dialog.exec())
    {
        User user;
        user.userName = dialog.getUserName();
        user.firstName = dialog.getFirstName();
        user.lastName = dialog.getLastName();
        for (auto game : dialog.getPreferredGames())
            user.preferredGame.insert(game, 0);

        if (!dialog.validateUser(user))
            continue;

        if (!data->addUser(user))
        {
            QString message = "User " + user.userName + " already exists";
            QMessageBox::critical(&dialog, "Error Add a User", message);
        }
        break;
    }
}

bool addUserDialog::validateUser(const User &user)
{
    auto reportError = [&](const QString &message)
    {
        QMessageBox::critical(this, "Error Add a User", message);
    };

    QString userName = user.userName;
    if (userName.isEmpty() || userName.size() > 16 || userName.contains(" "))
    {
        reportError("Username must be between 1 and 16 characters and don't contain invisible characters");
        return false;
    }

    QString firstName = user.firstName;
    if (firstName.isEmpty() || !firstName.contains(QRegularExpression("^[A-Z]")) ||
        !firstName.contains(QRegularExpression("^[A-Za-z]+$")))
    {
        reportError("First name must consist of Latin characters only and starts with a capital letter.");
        return false;
    }

    QString lastName = user.lastName;
    if (lastName.isEmpty() || !lastName.contains(QRegularExpression("^[A-Z]")) ||
        !lastName.contains(QRegularExpression(R"(^[A-Za-z]+('?[A-Za-z]+)*$)")))
    {
        reportError("Last name must consist of Latin characters only and starts with a capital letter.");
        return false;
    }

    auto preferredGames = user.preferredGame;
    if (preferredGames.isEmpty())
    {
        reportError("You must select at least one game");
        return false;
    }

    return true;
}
