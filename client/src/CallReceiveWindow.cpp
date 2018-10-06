#include "CallReceiveWindow.hpp"
#include "ui_CallReceiveWindow.h"
#include "moc_CallReceiveWindow.cpp"

CallReceiveWindow::CallReceiveWindow(std::string const &name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CallReceiveWindow)
{
    ui->setupUi(this);
    ui->CallReceiveLabel->setText("Call from " + QString(name.c_str()));
}

CallReceiveWindow::~CallReceiveWindow()
{
    delete ui;
}

void CallReceiveWindow::on_AcceptButton_clicked()
{
    // Call Window -> Constructor parameter ?
}

void CallReceiveWindow::on_DeclineButton_clicked()
{
    // Send Message Call has been declined
    this->close();
}
