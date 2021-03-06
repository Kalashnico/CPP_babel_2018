#include "CallWindow.hpp"
#include "ui_CallWindow.h"
#include "moc_CallWindow.cpp"
#include <iostream>
#include <MainWindow.hpp>

CallWindow::CallWindow(QString contactCalled, QWidget *parent) : QMainWindow(
	parent), ui(new Ui::CallWindow), _contactCalled(contactCalled)
{
	ui->setupUi(this);
    ui->CallLabel->setText("Calling " + _contactCalled);
	// Setup Call
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
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
	auto parent = dynamic_cast<MainWindow *>(this->parent());
	parent->setInCall(false);
	protocol::callMessage endCallMessage;
	endCallMessage.headerId = protocol::END_CALL;
	strcpy(endCallMessage.clientName, _username.c_str());
	strcpy(endCallMessage.contactName, _contactCalled.toStdString().c_str());
	_tcpClient->send(endCallMessage);
	_tcpClient->receive();

	this->close();
}

void CallWindow::setUsername(std::string &username)
{
	this->_username = username;
}

void CallWindow::setContactName(std::string &name)
{
    this->_contactCalled = QString(name.c_str());
    ui->CallLabel->setText("Calling " + _contactCalled);
}

void CallWindow::setTcpClient(tcpclient::TcpClient *tcpClient)
{
	this->_tcpClient = tcpClient;
}
