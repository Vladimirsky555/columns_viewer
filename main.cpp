#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Стили для всего проекта
//	QFile styleFile(":/qss/styles.qss");
//	styleFile.open(QFile::ReadOnly | QFile::Text);
//	QTextStream styleStream(&styleFile);
//	QString styleSheet = styleStream.readAll();
//	qApp->setStyleSheet(styleSheet);

	MainWindow w;
	w.show();

	return a.exec();
}
