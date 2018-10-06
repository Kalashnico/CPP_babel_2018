#include <CallWindow.hpp>
#include <iostream>
#include "CallReceiveWindow.hpp"
#include "ui_CallReceiveWindow.h"
#include "moc_CallReceiveWindow.cpp"

CallReceiveWindow::CallReceiveWindow(std::string const &name, CallWindow *callWindow, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::CallReceiveWindow), _callWindow(callWindow)
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
	this->_callWindow->show();
	this->close();
	std::cout << "ACCEPT" << std::endl;
}

void CallReceiveWindow::setName(std::string name)
{
	ui->CallReceiveLabel->setText("Call from " + QString(name.c_str()));
}

void CallReceiveWindow::on_DeclineButton_clicked()
{
	std::cout << "ACCEPT" << std::endl;
	this->close();
}
