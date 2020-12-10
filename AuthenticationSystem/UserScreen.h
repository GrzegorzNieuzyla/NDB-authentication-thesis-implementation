#pragma once

#include <QWidget>

namespace Ui {
    class UserScreen;
}

class UserScreen : public QWidget
{
    Q_OBJECT

public:
    explicit UserScreen(QWidget *parent = nullptr);
    ~UserScreen();


    void setUsername(const QString& username);

private slots:
    void on_logoutButton_clicked();

    void on_exitButton_clicked();

    void on_changePasswordButton_clicked();

    void on_deleteAccountButton_clicked();

private:
    QString _username;
    Ui::UserScreen *ui;
};

