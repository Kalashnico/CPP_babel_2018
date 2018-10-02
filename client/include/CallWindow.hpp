#ifndef CALLWINDOW_H
#define CALLWINDOW_H

#include <QtWidgets/QMainWindow>

namespace Ui {
class CallWindow;
}

class CallWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CallWindow(QString contactCalled = "", QWidget *parent = nullptr);
    ~CallWindow();

private slots:
    void on_VolumeSlider_valueChanged(int value);
    void on_MuteCheckBox_stateChanged(int arg1);

    void on_HangUpButton_clicked();

private:
    Ui::CallWindow *ui;
    QString _contactCalled;
};

#endif // CALLWINDOW_H
