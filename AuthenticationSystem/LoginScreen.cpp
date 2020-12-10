#include "AuthenticationManager.h"
#include "LoginScreen.h"
#include "ui_LoginScreen.h"
#include "Config.h"
#include "UserScreen.h"
#include <QMessageBox>
#include <QStackedWidget>

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginScreen)
{
    ui->setupUi(this);
}

LoginScreen::~LoginScreen()
{
   delete ui;
}

void LoginScreen::on_exitButton_clicked()
{
    QApplication::exit();
}

void LoginScreen::on_loginButton_clicked()
{

    auto username = ui->loginEdit->text();
    auto password = ui->passwordEdit->text();
    AuthenticationManager authManager(DB_NAME);
    if (!authManager.CheckLogin(username, password))
    {
        QMessageBox msgBox;
        msgBox.setText("Login failed.");
        msgBox.exec();
        ui->passwordEdit->clear();
        return;
    }
    auto parent = dynamic_cast<QStackedWidget*>(parentWidget());
    UserScreen* userScreen = nullptr;
    for (auto child : parent->children())
    {
        userScreen = dynamic_cast<UserScreen*>(child);
        if (userScreen) break;
    }
    ui->passwordEdit->clear();
    ui->loginEdit->clear();
    userScreen->setUsername(username);
    parent->setCurrentIndex(2);
}

void LoginScreen::on_registerButton_clicked()
{
    dynamic_cast<QStackedWidget*>(parentWidget())->setCurrentIndex(1);
}
