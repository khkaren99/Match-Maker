#pragma once

#include <QDialog>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>

class addUserDialog : public QDialog
{
private:
    QLineEdit *userNameEdit;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QVector<QCheckBox *> gameCheckBoxes;

public:
    addUserDialog(const QStringList &games, QWidget *parent = nullptr);

    QString getUserName() const;
    QString getFirstName() const;
    QString getLastName() const;
    QStringList getPreferredGames() const;
};
