#include "MainWindow.hpp"
#include <CallWindow.hpp>
#include <QtWidgets/QApplication>
#include <QtGui/QFontDatabase>
#include <QtCore/QFile>
#include <LoginWindow.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    int id = QFontDatabase::addApplicationFont("../fonts/arial.ttf");
    if (id == -1)
        id = QFontDatabase::addApplicationFont("../../fonts/arial.ttf");
    if (id == -1)
        return 84;
    QString family = QFontDatabase::applicationFontFamilies(id).first();
	QFont ariel(family);

    LoginWindow w;
	w.show();

	return a.exec();
}
