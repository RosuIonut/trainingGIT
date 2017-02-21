#ifndef _MAPDRAW_H_
#define _MAPDRAW_H_

#include "MapImageDataServer.h"
#include "TileServer.h"
#include "Configs.h"

#include <QtCore>
#include <QtGui>
#include <QtDebug>

#include "DataStructures.h"

class MapDraw : public QWidget
{
	Q_OBJECT

public:
	MapDraw(QWidget* parent, int width, int height);
	~MapDraw();
	void setLocation(double longitude, double latitude) { setLocation(DoubleToMapCoord(longitude), DoubleToMapCoord(latitude)); }
	void setLocation(DT_MapCoordinate longitude, DT_MapCoordinate latitude);

	void zoomIn(QPoint);
	void zoomOut(QPoint);

	DT_stDrawProperties& getDrawProperties() {return *m_drawproperties;}

public slots:
	void onPositionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude);
	void initialize();
	void onImageSlide(int x, int y);
	void moveMap(int x, int y);

signals:
	void dataReady(void);
	void mouseSlide(int, int);
	
	void propChanged(DT_stDrawProperties*&);

private:
	// Current draw margins as coordinates
	CTileServer* m_tileServer;
	DT_stDrawProperties *m_drawproperties;
	QImage *m_image;
	MapImageDataServer *m_mapImageDataServer;

	QPoint imageStartPosition;

	void calcDrawProperties();
	void drawFullMap();

protected:
	void paintEvent (QPaintEvent *p_event);
	bool event(QEvent* event);
};

#endif //_MAPDRAW_H_
