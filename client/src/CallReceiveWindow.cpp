#include <CallWindow.hpp>
#include <MainWindow.hpp>
#include "CallReceiveWindow.hpp"
#include "ui_CallReceiveWindow.h"
#include "moc_CallReceiveWindow.cpp"
#include "SoundManager.hpp"

CallReceiveWindow::CallReceiveWindow(std::string const &name, CallWindow *callWindow, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::CallReceiveWindow), _callWindow(callWindow)
{
	ui->setupUi(this);
	ui->CallReceiveLabel->setText("Call from " + QString(name.c_str()));
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

CallReceiveWindow::~CallReceiveWindow()
{
	delete ui;
}

void CallReceiveWindow::on_AcceptButton_clicked()
{

	this->_udpClient->sendResponseDatagram(true);

	this->_callWindow->show();
	auto parent = dynamic_cast<MainWindow*>(this->parent());
	parent->setCalling(false);
	parent->setInCall(true);
	this->close();

	SoundManager *sm = new SoundManager(_udpClient);

}

void CallReceiveWindow::on_DeclineButton_clicked()
{
	auto parent = dynamic_cast<MainWindow*>(this->parent());
	parent->setCalling(false);
	parent->setInCall(false);
	this->_udpClient->sendResponseDatagram(false);
	this->close();
}

void CallReceiveWindow::setName(std::string name)
{
	ui->CallReceiveLabel->setText("Call from " + QString(name.c_str()));
}

void CallReceiveWindow::setUdpClient(udpclient::UdpClient *udpClient)
{
	this->_udpClient = udpClient;
}
