
#include <QApplication>
#include <QMainWindow>
#include <QDebug>

#include "NaviWindow.h"

/**
 *	Main: perform initializing, create window, draw
 *
 *
 **/
int
main(int argc, char** argv)
{
	// Every Qt app must have a QApplication.
	QApplication a(argc, argv);

    qDebug() << "OSM NAV Drawer 2012\n";
	qDebug() << "build: " __DATE__ " " __TIME__;

    NaviWindow *mainWindow = new NaviWindow(NULL, 480, 320);

    mainWindow->setWindowTitle("OSM NAV");
    mainWindow->show();
//    mainWindow->showFullScreen();

	return a.exec();
}
