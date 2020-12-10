#pragma once

#include "AuthenticationManager.h"

#include <QMainWindow>
#include <ui_MainWindow.h>

namespace Ui {
    class LoginScreen;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


private:
    Ui::MainWindow *ui;
};

