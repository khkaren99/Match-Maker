#pragma once

#include "dataManager.h"

#include <QDialog>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>

class addUserDialog : public QDialog
{
private:
    QLineEdit *m_userNameEdit;
    QLineEdit *m_firstNameEdit;
    QLineEdit *m_lastNameEdit;
    QVector<QCheckBox *> m_gameCheckBoxes;

public:
    addUserDialog(const QStringList &games, QWidget *parent = nullptr);

    QString getUserName() const;
    QString getFirstName() const;
    QString getLastName() const;
    QStringList getPreferredGames() const;

    static void addUser(DataManager *data);

private:
    bool validateUser(const User &);
};
