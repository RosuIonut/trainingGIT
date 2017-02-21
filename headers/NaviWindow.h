
#ifndef _NAVIWINDOW_H_
#define _NAVIWINDOW_H_

#include <QtGui>
#include <QtCore>
#include <MapDrawerFactory.h>
#include "MapDraw.h"
#include "MapController.h"
#include "GpsServiceIf.h"
#include "menu/headers/MainMenu.h"
#include "menu/headers/MenuButton.h"

class NaviWindow : public QMainWindow
{
	Q_OBJECT

public:
    NaviWindow(QWidget* parent = NULL, int iMapDrawWidth = 640, int iMapDrawHeight = 480);
	~NaviWindow();

private:
	MapController *mapController;
	MapDraw *mapDraw;
	GpsServiceIf* client;

	QPoint mouseStartPosition;
	QPoint mouseRelativePosition;
	QPoint imageStartPosition;

	QTimer mouseReleaseTimer;

	CMainMenu* m_MainMenu;
	CMenuButton* m_MenuButton;

	//map slide
	QTimer mapSlideTimer;
	QPoint slideDirection;
	int slideSteps;

public slots:
	void slideMap();
	void moveMap();

protected:
	void resizeEvent(QResizeEvent * ev);
	void mousePressEvent (QMouseEvent * e);
	void mouseReleaseEvent (QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent* event);
};



#endif //_NAVIWINDOW_H_
