#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>
#include "MainWindow.hpp"
#include "TcpClient.hpp"

namespace Ui {
	class LoginWindow;
}

class LoginWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit LoginWindow(tcpclient::TcpClient *,
			QWidget *parent = nullptr
		);

		~LoginWindow();

	private slots:

		void on_OkButton_clicked();

	private:
		Ui::LoginWindow *ui;
		tcpclient::TcpClient *_tcpClient;
		MainWindow *_mainWindow;
};

#endif // LOGINWINDOW_H
