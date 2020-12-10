#include "UserScreen.h"
#include "ui_UserScreen.h"
#include "Config.h"
#include <PasswordStrengthValidator.h>
#include <QMessageBox>
#include <QStackedWidget>
#include "AuthenticationManager.h"

UserScreen::UserScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserScreen)
{
    ui->setupUi(this);
}

UserScreen::~UserScreen()
{
    delete ui;
}

void UserScreen::on_logoutButton_clicked()
{
    dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentIndex(0);
}

void UserScreen::on_exitButton_clicked()
{
    QApplication::exit();
}

void UserScreen::on_changePasswordButton_clicked()
{
    auto password = ui->passwordEdit->text();
    if (password != ui->paswordConfirmEdit->text())
    {
        QMessageBox msgBox;
        msgBox.setText("Passwords do not match!");
        msgBox.exec();
        ui->passwordEdit->clear();
        ui->paswordConfirmEdit->clear();
        return;
    }
    PasswordStrengthValidator validator;
    if (!validator.Validate(password, _username))
    {
        QMessageBox msgBox;
        msgBox.setText("Password is not strong enough:" + validator.GetRequirements());
        msgBox.exec();
        ui->passwordEdit->clear();
        ui->paswordConfirmEdit->clear();
        return;
    }
    AuthenticationManager auth(DB_NAME);
    auth.ChangePassword(_username, password);

    QMessageBox msgBox;
    msgBox.setText("Password successfully changed");
    msgBox.exec();
    ui->passwordEdit->clear();
    ui->paswordConfirmEdit->clear();
}

void UserScreen::on_deleteAccountButton_clicked()
{
    auto resBtn = QMessageBox::question(nullptr, "Delete account confirmation" ,tr(("Delete account " + _username + "?\n").toStdString().c_str()),
        QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
        AuthenticationManager auth(DB_NAME);
        auth.DeleteAccount(_username);
        dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentIndex(0);
    }
}

void UserScreen::setUsername(const QString& username)
{
    _username = username;
    ui->usernamelabel->setText("Logged in as: " + username);
}

