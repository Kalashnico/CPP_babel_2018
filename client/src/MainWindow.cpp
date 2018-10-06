#include "MainWindow.hpp"
#include "CallWindow.hpp"
#include "ui_MainWindow.h"
#include "moc_MainWindow.cpp"
#include <iostream>
#include <QtWidgets/QListView>
#include <QtCore/QStringListModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <boost/algorithm/string.hpp>
#include <CallReceiveWindow.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	ui(new Ui::MainWindow), _protocol(new protocol::Protocol()),
	_tcpClient(nullptr)
{
	ui->setupUi(this);
	this->_callWindow = new CallWindow("", this);
	this->_callReceiveWindow = new CallReceiveWindow("", this->_callWindow);
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
		auto datagram = this->_udpClient->readPendingRequestDatagrams();

		if (datagram.caller.length() == 0)
			continue;

		std::cout << "Call request from: " << datagram.caller << std::endl;
		this->_callReceiveWindow->setName(datagram.caller);
		this->_callWindow->setName(datagram.caller);
		this->_callReceiveWindow->show();
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
	}

	this->_callWindow->show();
}
