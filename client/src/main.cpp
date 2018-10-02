#include "MainWindow.hpp"
#include <CallWindow.hpp>
#include <QtWidgets/QApplication>
#include <QtGui/QFontDatabase>
#include <QtCore/QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	int id = QFontDatabase::addApplicationFont("../../fonts/arial.ttf");
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	QFont arial(family);

	a.setFont(arial);

	MainWindow w;
	w.show();

	return a.exec();
}
