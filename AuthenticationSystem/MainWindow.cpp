#include "LoginScreen.h"
#include "MainWindow.h"
#include "UserScreen.h"
#include "ui_MainWindow.h"
#include <ui_LoginScreen.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
