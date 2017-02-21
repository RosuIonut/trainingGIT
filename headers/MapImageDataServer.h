#ifndef _MAPIMAGEDATASERVER_H_
#define _MAPIMAGEDATASERVER_H_

#include "TileServer.h"
#include "Configs.h"
#include "DataTypes.h"
#include "MapRegion.h"
#include "WayInfo.h"

extern "C" {
#include "MapInfo.h"
#include "FastUtils.h"
#include "MapUtils.h"
#include "StringParsers.h"
#include "DataInfo.h"
#include "DataStructures.h"
}

#include <QtCore>
#include <QtGui>
#include <QtDebug>

class MapImageDataServer : public QThread
{
    Q_OBJECT

public:
    MapImageDataServer(QWidget* parent, CTileServer* tileServer, DT_stDrawProperties *drawProperties, QImage *image);
	~MapImageDataServer();

	void loadMapSlice (DT_MapCoordinate _long_start, DT_MapCoordinate _long_stop, DT_MapCoordinate _lat_start, DT_MapCoordinate _lat_stop, int X, int Y, int width, int height);

	DT_MapUnit getMapSliceZoom() {return m_drawproperties->zoom;}
    DT_stDrawProperties *getMapDrawProperties() {return m_drawproperties;}
    void stop() {suicide=true;}
    void clear();

public slots:
	void mouseSlide(int, int);

signals:
	void dataReady();

public:
	CWayInfo* wayinfo;
	CMapInfo* mapinfo;
	CTileServer* m_tileServer;
	DT_stDrawProperties *m_drawproperties;
	QImage *m_image;
	QPainter *m_painter;

	bool suicide;
	int waysZoomLevel, areasZoomLevel, noDrawLevels, placesLevel;

	std::vector<MapRegion*> regions;

	void run();

	void renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, MapRegion*);
	void renderMapData (MapRegion*);
	void renderBBoxPolygons (int horiz_bbox_no, int vert_bbox_no, int zoom_level, MapRegion*);
	void renderBBoxBoundaries (int horiz_bbox_no, int vert_bbox_no, MapRegion* mapRegion);
	void drawStreetName(MapRegion* mapRegion, int, int, int);
	void drawPOIs(MapRegion* mapRegion,int,int);
	void drawPlaces(MapRegion* mapRegion,int,int,int);
};

#endif //_MAPIMAGEDATASERVER_H_
