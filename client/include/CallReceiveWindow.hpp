#pragma once

#include <QtWidgets/QMainWindow>
#include <CallReceiveWindow.hpp>
#include <CallWindow.hpp>

namespace Ui {
	class CallReceiveWindow;
}

class CallReceiveWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit CallReceiveWindow(std::string const &name,
			CallWindow *callWindow,
			QWidget *parent = nullptr
		);

		~CallReceiveWindow();
		void setName(std::string name);

	private slots:
		void on_AcceptButton_clicked();
		void on_DeclineButton_clicked();

	private:
		Ui::CallReceiveWindow *ui;
		CallWindow *_callWindow;
};
