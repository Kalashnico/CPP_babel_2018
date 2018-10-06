#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidgetItem>
#include "TcpClient.hpp"
#include "UdpClient.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = nullptr);

		~MainWindow();

		void setTcpClient(tcpclient::TcpClient *tcp);

		void setUsername(const std::string &username);

		void setPort(unsigned short port);

		void refreshContacts();

	private slots:

		void on_Contacts_itemClicked(QListWidgetItem *item);

		void on_RefreshButton_clicked();

		void CallAction();

	private:
		Ui::MainWindow *ui;
		QString _selectedContact;
		QStringList _contacts;

		protocol::Protocol *_protocol;
		tcpclient::TcpClient *_tcpClient;
		udpclient::UdpClient *_udpClient;
		std::string _username;
		unsigned short _port;
		std::thread *_thread;

		bool _calling;
		bool _inCall;

		void backgroundThread();
};

#endif // MAINWINDOW_H
