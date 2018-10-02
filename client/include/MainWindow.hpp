#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Contacts_itemClicked(QListWidgetItem *item);
    void on_RefreshButton_clicked();
    void CallAction();

private:
    Ui::MainWindow *ui;
    QString _selectedContact;

};

#endif // MAINWINDOW_H
