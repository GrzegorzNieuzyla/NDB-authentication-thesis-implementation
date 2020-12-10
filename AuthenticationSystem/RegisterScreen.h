#pragma once

#include <QWidget>

namespace Ui {
    class RegisterScreen;
}

class RegisterScreen : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterScreen(QWidget *parent = nullptr);
    ~RegisterScreen();

private slots:
    void on_exitButton_clicked();

    void on_loginButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::RegisterScreen *ui;
};

