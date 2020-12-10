#pragma once

#include <QWidget>

namespace Ui {
    class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

private slots:
    void on_exitButton_clicked();

    void on_loginButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::LoginScreen *ui;
};

