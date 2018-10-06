#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidgetItem>
#include "TcpClient.hpp"
#include "UdpClient.hpp"
#include <CallReceiveWindow.hpp>
#include <CallWindow.hpp>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = nullptr);

		~MainWindow();

		// Connection Related
		void setTcpClient(tcpclient::TcpClient *tcp);
		void setUsername(const std::string &username);
		void setPort(unsigned short port);
		void refreshContacts();

	private slots:
		void on_Contacts_itemClicked(QListWidgetItem *item);
		void on_RefreshButton_clicked();
		void CallAction();

	private:
		// UI
		Ui::MainWindow *ui;
		CallReceiveWindow *_callReceiveWindow;
		CallWindow *_callWindow;

		// Contacts
		QString _selectedContact;
		QStringList _contacts;

		// Connection
		protocol::Protocol *_protocol;
		tcpclient::TcpClient *_tcpClient;
		udpclient::UdpClient *_udpClient;
		std::string _username;
		unsigned short _port;

		// Thread Related
		std::thread *_thread;
		void backgroundThread();


};

#endif // MAINWINDOW_H
