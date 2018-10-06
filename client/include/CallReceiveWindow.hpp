#ifndef CALLRECEIVEWINDOW_H
#define CALLRECEIVEWINDOW_H

#include <QtWidgets/QMainWindow>

namespace Ui {
class CallReceiveWindow;
}

class CallReceiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CallReceiveWindow(std::string const &name,QWidget *parent = nullptr);
    ~CallReceiveWindow();

private slots:
    void on_AcceptButton_clicked();
    void on_DeclineButton_clicked();

private:
    Ui::CallReceiveWindow *ui;
};

#endif // CALLRECEIVEWINDOW_H
