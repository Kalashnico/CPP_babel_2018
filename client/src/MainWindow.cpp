#include "MainWindow.hpp"
#include "CallWindow.hpp"
#include "ui_MainWindow.h"
#include "moc_MainWindow.cpp"
#include <iostream>
#include <QtWidgets/QListView>
#include <QtCore/QStringListModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _protocol(new protocol::Protocol())
{
    ui->setupUi(this);
    QStringList list;
    list.append("Contact 1");
    list.append("Contact 2");
    list.append("Contact 3");
    list.append("Contact 4");
    ui->Contacts->addItems(list);

}

MainWindow::~MainWindow()
{
	protocol::connectionMessage disconnectMessage;
	disconnectMessage.headerId = protocol::DISCONNECT;
	strcpy(disconnectMessage.clientName, _username.c_str());
	strcpy(disconnectMessage.ip, "");
	disconnectMessage.port = _port;
	_tcpClient->send(disconnectMessage);
	delete _protocol;
    delete ui;
}

void MainWindow::setTcpClient(tcpclient::TcpClient *tcp)
{
    this->_tcpClient = tcp;
}

void MainWindow::setUsername(const std::string &username)
{
    this->_username = username;
}

void MainWindow::setPort(unsigned short port)
{
    this->_port = port;
}

void MainWindow::on_RefreshButton_clicked()
{
    // Refresh List (See Constructor)
    std::cout << "Refresh" << std::endl;
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
    auto callwindow = new CallWindow(this->_selectedContact, this);
    callwindow->show();
}
