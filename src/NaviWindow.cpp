#include "NaviWindow.h"
#include "PosDotDraw.h"
#include "menu/headers/MenuServer.h"

//#define _NAVI_WIN_DEBUG_

NaviWindow::NaviWindow(QWidget* parent /*=NULL*/, int iQTMapDrawerWidth /*=640*/, int iQTMapDrawerHeight /*=480*/) :
	QMainWindow(parent),
	client(new GpsServiceIf("org.osmnavi.GpsService", "/", QDBusConnection::sessionBus(), 0))
{
	QDesktopWidget *desktop = QApplication::desktop();

	int position_x = (desktop->width() - iQTMapDrawerWidth) / 2;
	int position_y = (desktop->height() - iQTMapDrawerHeight) / 2;

	// resize and move window to desired coordinates
	resize(iQTMapDrawerWidth, iQTMapDrawerHeight);
	move(position_x, position_y);

	mapDraw=new MapDraw(this, iQTMapDrawerWidth, iQTMapDrawerHeight);
	mapController=new MapController(this, *mapDraw);
	mapController->addOverlay(new PosDotDraw(this, iQTMapDrawerWidth, iQTMapDrawerHeight));

	CMenuServer *menu = new CMenuServer(this,iQTMapDrawerWidth,iQTMapDrawerHeight);

	mouseReleaseTimer.setSingleShot(true);
	mouseReleaseTimer.setInterval(100);
	mapSlideTimer.setSingleShot(true);

	connect(&mouseReleaseTimer, SIGNAL(timeout()), this, SLOT(moveMap()));
	connect(&mapSlideTimer, SIGNAL(timeout()), this, SLOT(slideMap()));

	connect(client, SIGNAL(coordinatesChanged(double, double)), mapController, SLOT(onPositionChanged(double, double)));
}

NaviWindow::~NaviWindow()
{
	delete mapController;
	delete mapDraw;
}

void NaviWindow::resizeEvent(QResizeEvent *)
{

}

void NaviWindow::mousePressEvent (QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
    	mouseStartPosition=event->pos();
    	mouseReleaseTimer.stop();
    }

    mapSlideTimer.stop();
    slideDirection = QPoint(0, 0);
}

void NaviWindow::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mouseRelativePosition += imageStartPosition;
		imageStartPosition = QPoint(0, 0);
		mouseReleaseTimer.start();
		slideSteps=17;
		mapSlideTimer.start(30);
	}
	else if (event->button() == Qt::RightButton)
	{
		DT_MapCoordinate longitude = AddMapUnits(mapDraw->getDrawProperties().visibleMarg.long_start, (event->pos().x()*mapDraw->getDrawProperties().res_x));
		DT_MapCoordinate latitude = AddMapUnits(mapDraw->getDrawProperties().visibleMarg.lat_start, ((mapDraw->getDrawProperties().height - event->pos().y())*mapDraw->getDrawProperties().res_y));

		qDebug() << "Longitude:"<<MapCoordToDouble(longitude)<<"Latitude:"<<MapCoordToDouble(latitude);

		mapController->onPositionChanged(latitude, longitude);
	}
}

void NaviWindow::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton)
	{
		slideDirection=event->pos()-mouseStartPosition-imageStartPosition;
		imageStartPosition=event->pos()-mouseStartPosition;

		mapController->slide(imageStartPosition.x() + mouseRelativePosition.x(), imageStartPosition.y() + mouseRelativePosition.y());
	}
}

void NaviWindow::wheelEvent(QWheelEvent* event)
{
	if(event->delta() > 0)
		mapDraw->zoomIn(event->pos());
	else
		mapDraw->zoomOut(event->pos());
}

void NaviWindow::moveMap()
{
	int x=mouseRelativePosition.x() + slideDirection.x() * (slideSteps - 1) * slideSteps / 40;
	int y=mouseRelativePosition.y() + slideDirection.y() * (slideSteps - 1) * slideSteps / 40;

	mouseRelativePosition = - slideDirection * (slideSteps - 1) * slideSteps / 40;

	mapController->slide(mouseRelativePosition.x(), mouseRelativePosition.y());
	mapDraw->moveMap(x, y);
}

void NaviWindow::slideMap()
{
	if(!--slideSteps)
		return;

	mouseRelativePosition += slideDirection * slideSteps / 20;
	mapSlideTimer.start(30);

	mapController->slide(mouseRelativePosition.x(), mouseRelativePosition.y());
}
