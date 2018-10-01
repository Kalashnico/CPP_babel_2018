#include "CallWindow.hpp"
#include "ui_CallWindow.h"
#include <iostream>

CallWindow::CallWindow(QString contactCalled, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CallWindow),
    _contactCalled(contactCalled)
{
    ui->setupUi(this);
    this->setWindowTitle("Calling " + _contactCalled);
    // Setup Call
}

CallWindow::~CallWindow()
{
    delete ui;
}

void CallWindow::on_VolumeSlider_valueChanged(int value)
{
    // Change Call Volume
    std::cout << value << std::endl;
}

void CallWindow::on_MuteCheckBox_stateChanged(int value)
{
    // Mute Button
    std::cout << ((value == 2) ? "True" : "False") << std::endl;
}


void CallWindow::on_HangUpButton_clicked()
{
    // End Call
    this->close();
}
