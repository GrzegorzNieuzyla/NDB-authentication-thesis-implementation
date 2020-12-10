#include "MainWindow.h"
#include "RegisterScreen.h"
#include "ui_RegisterScreen.h"
#include "Config.h"
#include <PasswordStrengthValidator.h>
#include <QMessageBox>
#include <QStackedWidget>

RegisterScreen::RegisterScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterScreen)
{
    ui->setupUi(this);
}

RegisterScreen::~RegisterScreen()
{
    delete ui;
}

void RegisterScreen::on_exitButton_clicked()
{
    QApplication::exit();
}

void RegisterScreen::on_loginButton_clicked()
{
    dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentIndex(0);
}

void RegisterScreen::on_registerButton_clicked()
{
    auto password = ui->passwordEdit->text();
    auto username = ui->loginEdit->text();
    if (username.size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Username not provided.");
        msgBox.exec();
        ui->loginEdit->clear();
        return;
    }
    if (password != ui->confirmEdit->text())
    {
        QMessageBox msgBox;
        msgBox.setText("Passwords do not match!");
        msgBox.exec();
        ui->passwordEdit->clear();
        ui->confirmEdit->clear();
        return;
    }
    PasswordStrengthValidator validator;
    if (!validator.Validate(password, username))
    {
        QMessageBox msgBox;
        msgBox.setText("Password is not strong enough:" + validator.GetRequirements());
        msgBox.exec();
        ui->passwordEdit->clear();
        ui->confirmEdit->clear();
        return;
    }

    AuthenticationManager authManager(DB_NAME);
    if (!authManager.Register(username, password))
    {
        QMessageBox msgBox;
        msgBox.setText("User already exists.");
        msgBox.exec();
        ui->loginEdit->clear();
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Account created successfully");
    msgBox.exec();
    ui->passwordEdit->clear();
    ui->confirmEdit->clear();
    ui->loginEdit->clear();
    on_loginButton_clicked();
}
