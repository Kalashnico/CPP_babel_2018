#ifndef CALLWINDOW_H
#define CALLWINDOW_H

#include <QtWidgets/QMainWindow>
#include "TcpClient.hpp"

namespace Ui {
	class CallWindow;
}

class CallWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit CallWindow(QString contactCalled = "", QWidget *parent = nullptr);
		~CallWindow();

		void setUsername(std::string &username);
		void setContactName(std::string &name);
		void setTcpClient(tcpclient::TcpClient*);

	private slots:
		void on_VolumeSlider_valueChanged(int value);
		void on_MuteCheckBox_stateChanged(int arg1);
		void on_HangUpButton_clicked();

	private:
		Ui::CallWindow *ui;
		QString _contactCalled;

		std::string _username;
		tcpclient::TcpClient *_tcpClient;
};

#endif // CALLWINDOW_H
