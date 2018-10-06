#include "LoginWindow.hpp"
#include "ui_LoginWindow.h"
#include "moc_LoginWindow.cpp"
#include <MainWindow.hpp>
#include <iostream>

LoginWindow::LoginWindow(tcpclient::TcpClient *tcpClient, QWidget *parent)
	: QMainWindow(parent), ui(new Ui::LoginWindow), _tcpClient(tcpClient),
	_mainWindow(new MainWindow())
{
	ui->setupUi(this);
	ui->ConnectionFailedLabel->setVisible(false);
	ui->PortInput->setValidator(new QIntValidator(0, 65535, this));
	ui->LoginInput->setMaxLength(12);
}

LoginWindow::~LoginWindow()
{
	_mainWindow->close();
	delete _mainWindow;
	delete ui;
}

void LoginWindow::on_OkButton_clicked()
{
	auto port = ui->PortInput->text().toInt();
	auto username = ui->LoginInput->text().toStdString();
	bool success = true;

	if (username.empty() || port == 0)
		success = false;
	else {
		auto protocol = new protocol::Protocol();

		protocol::connectionMessage connectMessage;
		connectMessage.headerId = protocol::CONNECT;
		strcpy(connectMessage.clientName, username.c_str());
		strcpy(connectMessage.ip, "");
		connectMessage.port = port;

		success = _tcpClient->send(connectMessage);
		if (success) {
			auto receivedMessage = _tcpClient->receive();
			if (receivedMessage.response == 0)
				success = false;
		}

		delete protocol;
	}

	if (success) {
		this->_mainWindow->setPort(port);
		this->_mainWindow->setUsername(username);
		this->_mainWindow->setTcpClient(_tcpClient);
		this->_mainWindow->show();
		this->_mainWindow->refreshContacts();
		this->close();
	} else
		ui->ConnectionFailedLabel->setVisible(true);
}
