#include "LoginWindow.hpp"
#include "ui_LoginWindow.h"
#include "moc_LoginWindow.cpp"
#include <MainWindow.hpp>
#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->ConnectionFailedLabel->setVisible(false);
    ui->PasswordInput->setValidator(new QIntValidator(0, 65535, this));
}


LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_OkButton_clicked()
{
    std::cout << "Ok Button Pressed" << std::endl;

    bool success = true;

    if (success)
    {
        MainWindow *w = new MainWindow(nullptr);
        w->show();
        this->close();
    }
    else
        ui->ConnectionFailedLabel->setVisible(true);
}
