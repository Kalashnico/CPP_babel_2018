#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidgetItem>
#include "TcpClient.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(tcpclient::TcpClient*, std::string &username, unsigned short port, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Contacts_itemClicked(QListWidgetItem *item);
    void on_RefreshButton_clicked();
    void CallAction();

private:
    Ui::MainWindow *ui;
    QString _selectedContact;

    protocol::Protocol *_protocol;
    tcpclient::TcpClient *_tcpClient;
    std::string _username;
    unsigned short _port;

};

#endif // MAINWINDOW_H
