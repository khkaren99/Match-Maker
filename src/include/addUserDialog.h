#pragma once

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QVector>

class addUserDialog : public QDialog
{
    Q_OBJECT
public:
    addUserDialog(const QVector<QString>& games, QWidget *parent = nullptr);
    
    QString getUserName() const;
    QString getFirstName() const;
    QString getLastName() const;
    QStringList getPreferdGames() const;

private:
    QLineEdit *userNameEdit;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QVector<QCheckBox*> gameCheckBoxes;
};