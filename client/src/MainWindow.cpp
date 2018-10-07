#include "MainWindow.hpp"
#include "CallWindow.hpp"
#include "ui_MainWindow.h"
#include "moc_MainWindow.cpp"
#include <iostream>
#include <chrono>
#include <QtWidgets/QListView>
#include <QtCore/QStringListModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <boost/algorithm/string.hpp>
#include <CallReceiveWindow.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	ui(new Ui::MainWindow), _protocol(new protocol::Protocol()),
	_tcpClient(nullptr)
{
	ui->setupUi(this);
	this->_callWindow = new CallWindow("", this);
	this->_callReceiveWindow = new CallReceiveWindow("", this->_callWindow, this);
}


MainWindow::~MainWindow()
{
	if (_tcpClient != nullptr) {
		protocol::connectionMessage disconnectMessage;
		disconnectMessage.headerId = protocol::DISCONNECT;
		strcpy(disconnectMessage.clientName, _username.c_str());
		strcpy(disconnectMessage.ip, "");
		disconnectMessage.port = _port;
		_tcpClient->send(disconnectMessage);
		delete _tcpClient;
	}

	delete _protocol;
	delete ui;
}

void MainWindow::backgroundThread()
{
	while (1) {

		if (!_calling && !_inCall) {
			auto datagram = this->_udpClient->readPendingRequestDatagrams();

			if (datagram.caller.length() == 0)
				continue;

			this->_calling = true;

			this->_udpClient->setContactInfo(datagram.caller, datagram.ip, datagram.port);

			this->_callReceiveWindow->setName(datagram.caller);
			this->_callReceiveWindow->setUdpClient(this->_udpClient);
			this->_callWindow->setUsername(this->_username);
			this->_callWindow->setContactName(datagram.caller);
			this->_callWindow->setTcpClient(this->_tcpClient);
			this->_callReceiveWindow->show();
		}

		if (_calling) {
			auto response = this->_udpClient->readPendingResponseDatagrams();
			if (response != -1) {
				if (response == 1)
					_inCall = true;
				_calling = false;
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (this->_timeout++ == 10) {
				this->_timeout = 0;
				_calling = false;

				protocol::callMessage noReplyMessage;
				noReplyMessage.headerId = protocol::NO_REPLY;
				strcpy(noReplyMessage.clientName, _username.c_str());
				strcpy(noReplyMessage.contactName, this->_udpClient->getContactName().c_str());
				_tcpClient->send(noReplyMessage);
				_tcpClient->receive();

				this->_callWindow->close();
			}

		}
	}
}

void MainWindow::setTcpClient(tcpclient::TcpClient *tcp)
{
	this->_tcpClient = tcp;
	this->_udpClient = new udpclient::UdpClient(this->_username, this->_port);
	this->_thread = new std::thread(&MainWindow::backgroundThread, this);
}

void MainWindow::setUsername(const std::string &username)
{
	this->_username = username;
}

void MainWindow::setPort(unsigned short port)
{
	this->_port = port;
}

void MainWindow::refreshContacts()
{
	on_RefreshButton_clicked();
}

void MainWindow::on_RefreshButton_clicked()
{
	protocol::infoMessage refreshMessage;
	refreshMessage.headerId = protocol::GET_CONTACTS;
	_tcpClient->send(refreshMessage);
	auto response = _tcpClient->receiveClients();

	std::string contacts(response.contactNames);

	std::vector<std::string> splitContacts;
	boost::split(splitContacts, contacts, [](char c) {
		return c == '|';
	});

	_contacts.clear();
	for (auto contact : splitContacts) {
		if (contact != _username)
			_contacts.append(QString::fromStdString(contact));
	}

	ui->Contacts->clear();
	ui->Contacts->addItems(_contacts);
}

void MainWindow::on_Contacts_itemClicked(QListWidgetItem *item)
{
	this->_selectedContact = item->text();
	QMenu contextMenu(tr("Context menu"), this);
	QAction action1("Call", this);
	contextMenu.addAction(&action1);
	connect(&action1, &QAction::triggered, this, &MainWindow::CallAction);
	contextMenu.exec(QCursor::pos());
}

void MainWindow::CallAction()
{
	this->_callWindow = new CallWindow(this->_selectedContact, this);
	this->_callWindow->setTcpClient(this->_tcpClient);
	this->_callWindow->setUsername(this->_username);

	protocol::callMessage callRequestMessage;
	callRequestMessage.headerId = protocol::REQUEST_CALL;
	strcpy(callRequestMessage.clientName, _username.c_str());
	strcpy(callRequestMessage.contactName,
		_selectedContact.toStdString().c_str());
	_tcpClient->send(callRequestMessage);

	auto serverReponse = _tcpClient->receive();
	if (serverReponse.response == 1) {
		std::string contact(_selectedContact.toStdString());
		std::string ip(serverReponse.ip);
		_udpClient->setContactInfo(contact, ip, serverReponse.port);
		_udpClient->sendCallRequestDatagram();

		_calling = true;
		_timeout = 0;
		this->_callWindow->show();
	}
}

void MainWindow::setInCall(bool const &value)
{
	this->_inCall = value;
}

void MainWindow::setCalling(bool const &value)
{
	this->_calling = value;
}

#pragma clang diagnostic pop