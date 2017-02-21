#include "MapImageDataServer.h"
#include "DrawStyle.h"
#include <algorithm>
#include "MapUtils.h"
#include <QPainterPathStroker>

//#define _DEBUG_MAPIMAGEDATASERVER_
MapImageDataServer::MapImageDataServer(QWidget* parent, CTileServer* tileServer, DT_stDrawProperties *drawProperties, QImage *image) :
	m_tileServer(tileServer),
	m_drawproperties(drawProperties),
	m_image(image),
	m_painter(new QPainter(m_image))
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::MapImageDataServer";
#endif

	connect(this, SIGNAL(dataReady()), parent, SLOT(update()));
	connect(parent, SIGNAL(mouseSlide(int, int)), this, SLOT(mouseSlide(int, int)));

	m_painter->setRenderHint(QPainter::Antialiasing);
	m_painter->fillRect(0, 0, m_drawproperties->bufferWidth, m_drawproperties->bufferHeight, DrawStyle::getMapDrawStyle()->getBackColor());

    mapinfo = CMapInfo::instance();
    wayinfo = new CWayInfo();
}

MapImageDataServer::~MapImageDataServer()
{
	clear();
	delete m_painter;
	delete wayinfo;
}

void MapImageDataServer::loadMapSlice (DT_MapCoordinate _long_start, DT_MapCoordinate _long_stop, DT_MapCoordinate _lat_start, DT_MapCoordinate _lat_stop, int X, int Y, int width, int height)
{
	DT_MapCoordinate x1, x2, y1, y2;
	int x, y, w, h;
	MapRegion *reg;

	//visible area
	x1=qMax(_long_start, m_drawproperties->visibleMarg.long_start);
	x2=qMin(_long_stop, m_drawproperties->visibleMarg.long_stop);
	y1=qMax(_lat_start, m_drawproperties->visibleMarg.lat_start);
	y2=qMin(_lat_stop, m_drawproperties->visibleMarg.lat_stop);

	if(x1<x2 && y1<y2)
	{
		x=qMax(X, m_drawproperties->startX);
		y=qMax(Y, m_drawproperties->startY);
		w=qMin(X+width, m_drawproperties->startX+m_drawproperties->width) - x;
		h=qMin(Y+height, m_drawproperties->startY+m_drawproperties->height) - y;

		reg=new MapRegion(x, y, w, h, 0, true);
		reg->setMargins(x1, x2, y1, y2);
		regions.push_back(reg);
	}

	//top buffer
	x1=qMax(_long_start, m_drawproperties->visibleMarg.long_start);
	x2=qMin(_long_stop, m_drawproperties->visibleMarg.long_stop);
	y1=qMax(_lat_start, m_drawproperties->visibleMarg.lat_stop);
	y2=qMin(_lat_stop, m_drawproperties->bufferMarg.lat_stop);

	if(x1<x2 && y1<y2)
	{
		x=qMax(X, m_drawproperties->startX);
		y=qMax(Y, 0);
		w=qMin(X+width, m_drawproperties->startX+m_drawproperties->width) - x;
		h=qMin(Y+height, m_drawproperties->startY) - y;

		reg=new MapRegion(x, y, w, h, 0, false);
		reg->setMargins(x1, x2, y1, y2);
		regions.push_back(reg);
	}

	//bottom buffer
	x1=qMax(_long_start, m_drawproperties->visibleMarg.long_start);
	x2=qMin(_long_stop, m_drawproperties->visibleMarg.long_stop);
	y1=qMax(_lat_start, m_drawproperties->bufferMarg.lat_start);
	y2=qMin(_lat_stop, m_drawproperties->visibleMarg.lat_start);

	if(x1<x2 && y1<y2)
	{
		x=qMax(X, m_drawproperties->startX);
		y=qMax(Y, m_drawproperties->startY+m_drawproperties->height);
		w=qMin(X+width, m_drawproperties->startX+m_drawproperties->width) - x;
		h=qMin(Y+height, m_drawproperties->bufferHeight) - y;

		reg=new MapRegion(x, y, w, h, 0, false);
		reg->setMargins(x1, x2, y1, y2);
		regions.push_back(reg);
	}

	//right buffer
	x1=qMax(_long_start, m_drawproperties->bufferMarg.long_start);
	x2=qMin(_long_stop, m_drawproperties->visibleMarg.long_start);
	y1=qMax(_lat_start, m_drawproperties->bufferMarg.lat_start);
	y2=qMin(_lat_stop, m_drawproperties->bufferMarg.lat_stop);

	if(x1<x2 && y1<y2)
	{
		x=qMax(X, 0);
		y=qMax(Y, 0);
		w=qMin(X+width, m_drawproperties->startX) - x;
		h=qMin(Y+height, m_drawproperties->bufferHeight) - y;

		reg=new MapRegion(x, y, w, h, 0, false);
		reg->setMargins(x1, x2, y1, y2);
		regions.push_back(reg);
	}

	//right buffer
	x1=qMax(_long_start, m_drawproperties->visibleMarg.long_stop);
	x2=qMin(_long_stop, m_drawproperties->bufferMarg.long_stop);
	y1=qMax(_lat_start, m_drawproperties->bufferMarg.lat_start);
	y2=qMin(_lat_stop, m_drawproperties->bufferMarg.lat_stop);

	if(x1<x2 && y1<y2)
	{
		x=qMax(X, m_drawproperties->startX+m_drawproperties->width);
		y=qMax(Y, 0);
		w=qMin(X+width, m_drawproperties->bufferWidth) - x;
		h=qMin(Y+height, m_drawproperties->bufferHeight) - y;

		reg=new MapRegion(x, y, w, h, 0, false);
		reg->setMargins(x1, x2, y1, y2);
		regions.push_back(reg);
	}
}

void MapImageDataServer::mouseSlide(int X, int Y)
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::mouseSlide";
#endif

    m_painter->drawImage(X, Y, m_image->copy());
    if(X<0)
	{
		m_painter->fillRect(m_drawproperties->bufferWidth+X, 0, -X, m_drawproperties->bufferHeight, QColor(0xeffdde));

		if(Y<0)
			m_painter->fillRect(0, m_drawproperties->bufferHeight+Y, m_drawproperties->bufferWidth+X, -Y, QColor(0xeffdde));
		else
			m_painter->fillRect(0, 0, m_drawproperties->bufferWidth+X, Y, QColor(0xeffdde));
	}
	else
	{
		m_painter->fillRect(0, 0, X, m_drawproperties->bufferHeight, QColor(0xeffdde));

		if(Y<0)
			m_painter->fillRect(X, m_drawproperties->bufferHeight+Y, m_drawproperties->bufferWidth-X, -Y, QColor(0xeffdde));
		else
			m_painter->fillRect(X, 0, m_drawproperties->bufferWidth-X, Y, QColor(0xeffdde));
	}

	std::vector<MapRegion*> tmpRegions(regions);
	regions.clear();

	for(unsigned int i=0;i<tmpRegions.size();i++)
	{
		MapRegion *fullReg=tmpRegions[i];
		MapRegion *reg;

		DT_MapCoordinate x1, x2, y1, y2;
		int x, y, w, h;

		x1=qMax(fullReg->marg.coord.long_start, m_drawproperties->visibleMarg.long_start);
		x2=qMin(fullReg->marg.coord.long_stop, m_drawproperties->visibleMarg.long_stop);
		y1=qMax(fullReg->marg.coord.lat_start, m_drawproperties->visibleMarg.lat_start);
		y2=qMin(fullReg->marg.coord.lat_stop, m_drawproperties->visibleMarg.lat_stop);

		if(x1<x2 && y1<y2)
		{
			x=qMax(fullReg->startX+X, m_drawproperties->startX);
			y=qMax(fullReg->startY+Y, m_drawproperties->startY);
			w=qMin(fullReg->startX+X+fullReg->width, m_drawproperties->startX+m_drawproperties->width) - x;
			h=qMin(fullReg->startY+Y+fullReg->height, m_drawproperties->startY+m_drawproperties->height) - y;

			if(w<=0 || h<=0)
			{
				qDebug().space()<<"     CENTER"<<x<<y<<w<<h;
				return;
			}

			reg=new MapRegion(x, y, w, h, fullReg->drawLevel, true);
			reg->setMargins(x1, x2, y1, y2);
			reg->painter.drawImage(0, 0, fullReg->image, qMax(x-fullReg->startX-X, 0), qMax(y-fullReg->startY-Y, 0), w, h);
			regions.push_back(reg);
		}

		//top buffer
		x1=qMax(fullReg->marg.coord.long_start, m_drawproperties->visibleMarg.long_start);
		x2=qMin(fullReg->marg.coord.long_stop, m_drawproperties->visibleMarg.long_stop);
		y1=qMax(fullReg->marg.coord.lat_start, m_drawproperties->visibleMarg.lat_stop);
		y2=qMin(fullReg->marg.coord.lat_stop, m_drawproperties->bufferMarg.lat_stop);

		if(x1<x2 && y1<y2)
		{
			x=qMax(fullReg->startX+X, m_drawproperties->startX);
			y=qMax(fullReg->startY+Y, 0);
			w=qMin(fullReg->startX+X+fullReg->width, m_drawproperties->startX+m_drawproperties->width) - x;
			h=qMin(fullReg->startY+Y+fullReg->height, m_drawproperties->startY) - y;

			if(w<=0 || h<=0)
			{
				qDebug().space()<<"     TOP"<<x<<y<<w<<h;
				return;
			}

			reg=new MapRegion(x, y, w, h, fullReg->drawLevel, false);
			reg->setMargins(x1, x2, y1, y2);
			reg->painter.drawImage(0, 0, fullReg->image, qMax(x-fullReg->startX-X, 0), qMax(y-fullReg->startY-Y, 0), w, h);
			regions.push_back(reg);
		}

		//bottom buffer
		x1=qMax(fullReg->marg.coord.long_start, m_drawproperties->visibleMarg.long_start);
		x2=qMin(fullReg->marg.coord.long_stop, m_drawproperties->visibleMarg.long_stop);
		y1=qMax(fullReg->marg.coord.lat_start, m_drawproperties->bufferMarg.lat_start);
		y2=qMin(fullReg->marg.coord.lat_stop, m_drawproperties->visibleMarg.lat_start);

		if(x1<x2 && y1<y2)
		{
			x=qMax(fullReg->startX+X, m_drawproperties->startX);
			y=qMax(fullReg->startY+Y, m_drawproperties->startY+m_drawproperties->height);
			w=qMin(fullReg->startX+X+fullReg->width, m_drawproperties->startX+m_drawproperties->width) - x;
			h=qMin(fullReg->startY+Y+fullReg->height, m_drawproperties->bufferHeight) - y;

			if(w<=0 || h<=0)
				{
					qDebug().space()<<"     BOTTOM"<<x<<y<<w<<h;
					return;
				}

			reg=new MapRegion(x, y, w, h, fullReg->drawLevel, false);
			reg->setMargins(x1, x2, y1, y2);
			reg->painter.drawImage(0, 0, fullReg->image, qMax(x-fullReg->startX-X, 0), qMax(y-fullReg->startY-Y, 0), w, h);
			regions.push_back(reg);
		}

		//right buffer
		x1=qMax(fullReg->marg.coord.long_start, m_drawproperties->bufferMarg.long_start);
		x2=qMin(fullReg->marg.coord.long_stop, m_drawproperties->visibleMarg.long_start);
		y1=qMax(fullReg->marg.coord.lat_start, m_drawproperties->bufferMarg.lat_start);
		y2=qMin(fullReg->marg.coord.lat_stop, m_drawproperties->bufferMarg.lat_stop);

		if(x1<x2 && y1<y2)
		{
			x=qMax(fullReg->startX+X, 0);
			y=qMax(fullReg->startY+Y, 0);
			w=qMin(fullReg->startX+X+fullReg->width, m_drawproperties->startX) - x;
			h=qMin(fullReg->startY+Y+fullReg->height, m_drawproperties->bufferHeight) - y;

			if(w<=0 || h<=0)
				{
					qDebug().space()<<"     RIGHT"<<x<<y<<w<<h;
					return;
				}

			reg=new MapRegion(x, y, w, h, fullReg->drawLevel, false);
			reg->setMargins(x1, x2, y1, y2);
			reg->painter.drawImage(0, 0, fullReg->image, qMax(x-fullReg->startX-X, 0), qMax(y-fullReg->startY-Y, 0), w, h);
			regions.push_back(reg);
		}

		//right buffer
		x1=qMax(fullReg->marg.coord.long_start, m_drawproperties->visibleMarg.long_stop);
		x2=qMin(fullReg->marg.coord.long_stop, m_drawproperties->bufferMarg.long_stop);
		y1=qMax(fullReg->marg.coord.lat_start, m_drawproperties->bufferMarg.lat_start);
		y2=qMin(fullReg->marg.coord.lat_stop, m_drawproperties->bufferMarg.lat_stop);

		if(x1<x2 && y1<y2)
		{
			x=qMax(fullReg->startX+X, m_drawproperties->startX+m_drawproperties->width);
			y=qMax(fullReg->startY+Y, 0);
			w=qMin(fullReg->startX+X+fullReg->width, m_drawproperties->bufferWidth) - x;
			h=qMin(fullReg->startY+Y+fullReg->height, m_drawproperties->bufferHeight) - y;

			if(w<=0 || h<=0)
				{
					qDebug().space()<<"     LEFT"<<x<<y<<w<<h;
					return;
				}

			reg=new MapRegion(x, y, w, h, fullReg->drawLevel, false);
			reg->setMargins(x1, x2, y1, y2);
			reg->painter.drawImage(0, 0, fullReg->image, qMax(x-fullReg->startX-X, 0), qMax(y-fullReg->startY-Y, 0), w, h);
			regions.push_back(reg);
		}

		delete fullReg;
	}
}

void MapImageDataServer::clear()
{
	stop();
	wait();

	for(unsigned int i=0; i<regions.size(); i++)
		delete regions[i];

	regions.clear();
}

void MapImageDataServer::renderBBoxPolygons (int horiz_bbox_no, int vert_bbox_no, int zoom_level, MapRegion* mapRegion)
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::renderBBoxPolygons";
#endif

    unsigned int bboxid = horiz_bbox_no + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

    // Check the tile server status
	m_tileServer->setBBox(bboxid);
	m_tileServer->setZoomLevel(zoom_level);
	if(m_tileServer->LoadBBox())
		return;

	for(DT_OSMAreaBBox* bbox_area = m_tileServer->getFirstArea(); bbox_area; bbox_area = m_tileServer->getNextArea())
    {
    	int new_vert_bbox_no = vert_bbox_no;
		int new_horiz_bbox_no = horiz_bbox_no;
    	unsigned int newBBox, prevNodeId;
    	bool isDrawn=false;

    	DT_OSMAreaBBox* area=bbox_area;
    	while(area->nextBBoxId)
    	{
    		newBBox=area->nextBBoxId-1;
    		prevNodeId=area->nextBBoxNodeId;

    		new_vert_bbox_no = newBBox / mapinfo->getMapInfo()->horBBoxNo;
    		new_horiz_bbox_no = newBBox - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;

    		if(newBBox<bboxid && new_vert_bbox_no>=mapRegion->marg.bbox.vert_start && new_horiz_bbox_no>=mapRegion->marg.bbox.horiz_start)
    		{
    			isDrawn=true;
    			break;
    		}

    		m_tileServer->setBBox(newBBox);
			m_tileServer->LoadBBox();

			int areaCnt = m_tileServer->countAreas(area->id, newBBox, area->zoomLevel);
			if(areaCnt==0)
			{
				isDrawn=true;
				break;
			}
    		while(areaCnt--)
    		{
    			area = m_tileServer->getArea(area->id, newBBox, area->zoomLevel, areaCnt);
				if (area->prevBBoxNodeId == prevNodeId)
					break;
    		}
    	}

    	if(isDrawn)
		{
			m_tileServer->setBBox(bboxid);
			continue;
		}

    	while(bbox_area->prevBBoxId)
		{
			newBBox=bbox_area->prevBBoxId-1;
			prevNodeId=bbox_area->prevBBoxNodeId;

			new_vert_bbox_no = newBBox / mapinfo->getMapInfo()->horBBoxNo;
			new_horiz_bbox_no = newBBox - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;

			if(newBBox<bboxid && new_vert_bbox_no>=mapRegion->marg.bbox.vert_start && new_horiz_bbox_no>=mapRegion->marg.bbox.horiz_start)
			{
				isDrawn=true;
				break;
			}

			m_tileServer->setBBox(newBBox);
			m_tileServer->LoadBBox();

			int areaCnt = m_tileServer->countAreas(bbox_area->id, newBBox, bbox_area->zoomLevel);
			if(areaCnt==0)
			{
				isDrawn=true;
				break;
			}
			while(areaCnt--)
			{
				bbox_area = m_tileServer->getArea(bbox_area->id, newBBox, bbox_area->zoomLevel, areaCnt);
				if (bbox_area->nextBBoxNodeId == prevNodeId)
					break;
			}
		}

    	if(isDrawn)
		{
			m_tileServer->setBBox(bboxid);
			continue;
		}

    	int tmpdrawcoord_x = 0, tmpdrawcoord_y = 0;
    	int lastx = 0, lasty = 0;

    	int horiz_coordoffset;
        if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
        	horiz_coordoffset = new_horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
        else
        	horiz_coordoffset = new_horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

    	int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (new_vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));
        QPolygon polygon;

        for (int i = 0; i < bbox_area->referencesNo; i++)
		{
			DT_BBoxNode *node = m_tileServer->getNode(bbox_area->referenceList[i]);

			if(node==NULL)
			{
				continue;
			}

			tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
			tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

			if(bbox_area->type == 47 || ABS(lastx-tmpdrawcoord_x)+ABS(lasty-tmpdrawcoord_y)>4)
			{
				lastx = tmpdrawcoord_x;
				lasty = tmpdrawcoord_y;
				polygon << QPoint(tmpdrawcoord_x, tmpdrawcoord_y);
			}
		}

        while(bbox_area->nextBBoxId)
		{
        	newBBox=bbox_area->nextBBoxId-1;
        	prevNodeId=bbox_area->nextBBoxNodeId;

        	new_vert_bbox_no = newBBox / mapinfo->getMapInfo()->horBBoxNo;
			new_horiz_bbox_no = newBBox - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;

			m_tileServer->setBBox(newBBox);
			m_tileServer->LoadBBox();

			int areaCnt = m_tileServer->countAreas(bbox_area->id, newBBox, bbox_area->zoomLevel);

			if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
				horiz_coordoffset = new_horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
			else
				horiz_coordoffset = new_horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);
			vert_coordoffset = (mapRegion->marg.bbox.vert_size - (new_vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));

			while(areaCnt--)
			{
				bbox_area = m_tileServer->getArea(bbox_area->id, newBBox, bbox_area->zoomLevel, areaCnt);
				if (bbox_area->prevBBoxNodeId == prevNodeId)
				{
					for (int i = 0; i < bbox_area->referencesNo; i++)
					{
						DT_BBoxNode *node = m_tileServer->getNode(bbox_area->referenceList[i]);

						tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
						tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

						if(bbox_area->type == 47 || ABS(lastx-tmpdrawcoord_x)+ABS(lasty-tmpdrawcoord_y)>4)
						{
							lastx = tmpdrawcoord_x;
							lasty = tmpdrawcoord_y;
							polygon << QPoint(tmpdrawcoord_x, tmpdrawcoord_y);
						}
					}

					break;
				}
			}
		}

        if(suicide) return;

		mapRegion->painter.setBrush(DrawStyle::getMapDrawStyle()->setAreaBrushStyle(bbox_area->type));

        mapRegion->painter.drawPolygon(polygon);
        m_tileServer->setBBox(bboxid);
    }
}

void MapImageDataServer::drawPOIs(MapRegion* mapRegion, int horiz_bbox_no, int vert_bbox_no)
{
	int bboxid = horiz_bbox_no + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

	int horiz_coordoffset;
	if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
		horiz_coordoffset = horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
	else
		horiz_coordoffset = horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

	int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));
	int tmpdrawcoord_x,tmpdrawcoord_y;
	m_tileServer->setBBox(bboxid);

	for(DT_OSMPoi* bbox_poi = m_tileServer->getFirstPOI(); bbox_poi != NULL; bbox_poi = m_tileServer->getNextPOI())
	{
		DT_BBoxNode* node = m_tileServer->getNode(bbox_poi->id);

		tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) / m_drawproperties->res_x;
		tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) / m_drawproperties->res_y;

		QString path = DrawStyle::getMapDrawStyle()->getPOIIcon(bbox_poi->type,bbox_poi->object);

		if(suicide) return;

		mapRegion->painter.setPen(DrawStyle::getMapDrawStyle()->getPOIColor(bbox_poi->type));
		mapRegion->painter.drawImage(tmpdrawcoord_x-10, tmpdrawcoord_y-20, QImage(path));

		if(bbox_poi->name != NULL && m_drawproperties->zoom < 150)
			mapRegion->painter.drawText(tmpdrawcoord_x-24, tmpdrawcoord_y+17, 60, 100, Qt::AlignHCenter | Qt::TextWordWrap, bbox_poi->name);
	}
}

void MapImageDataServer::drawPlaces(MapRegion* mapRegion, int horiz_bbox_no, int vert_bbox_no, int type)
{

	int bboxid = horiz_bbox_no + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

	int horiz_coordoffset;
	if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
		horiz_coordoffset = horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
	else
		horiz_coordoffset = horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

	int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));
	int tmpdrawcoord_x,tmpdrawcoord_y;
	m_tileServer->setBBox(bboxid);

	for(DT_MapNodePlaces* place = m_tileServer->getFirstPlace(type); place != NULL; place = m_tileServer->getNextPlace(type))
	{
		DT_BBoxNode* node = m_tileServer->getNode(place->id);

		tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
		tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

		if(suicide) return;

		QPainterPath path;
		QPainterPathStroker stroker;
		stroker.setWidth(1);

		path.addText(tmpdrawcoord_x, tmpdrawcoord_y, QFont("Arial", DrawStyle::getMapDrawStyle()->getPlaceSize(type), QFont::Bold), place->name);
		mapRegion->painter.setPen(Qt::white);
		mapRegion->painter.setBrush(Qt::black);
		mapRegion->painter.drawPath(stroker.createStroke(path));

		mapRegion->painter.setPen(Qt::NoPen);
		mapRegion->painter.drawPath(path);
	}
}

void MapImageDataServer::drawStreetName(MapRegion* mapRegion, int horiz_bbox_no, int vert_bbox_no, int zoomLevel)
{
	int bboxid = horiz_bbox_no  + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

	// Check the tile server status
	m_tileServer->setBBox(bboxid);
	m_tileServer->setZoomLevel(zoomLevel);
	if(m_tileServer->LoadBBox())
		return;

	int horiz_coordoffset;
    if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
    else
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

    int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));

    for(DT_OSMWayBBox* bbox_way = m_tileServer->getFirstWay(); bbox_way != NULL; bbox_way = m_tileServer->getNextWay())
	{
		DT_OSMWayInfo *info = m_tileServer->getWayInfo(bboxid,bbox_way->id);
		if(info==NULL || info->name==NULL)
			continue;

		int tmpdrawcoord_x, tmpdrawcoord_y;
		int tmpdrawcoordAux_x, tmpdrawcoordAux_y;
		QPoint A, B;
		int length=0;

		DT_BBoxNode *node, *prevNode = m_tileServer->getNode(bbox_way->referenceList[0]);
		tmpdrawcoordAux_x = ((prevNode->bbox_long + horiz_coordoffset)) / m_drawproperties->res_x;
		tmpdrawcoordAux_y = ((vert_coordoffset - prevNode->bbox_lat)) / m_drawproperties->res_y;

		for (int i = 1; i < bbox_way->referencesNo; i++, prevNode=node)
		{
			node = m_tileServer->getNode(bbox_way->referenceList[i]);

			tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
			tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

			int tmpLength=(tmpdrawcoord_x-tmpdrawcoordAux_x)*(tmpdrawcoord_x-tmpdrawcoordAux_x)+(tmpdrawcoord_y-tmpdrawcoordAux_y)*(tmpdrawcoord_y-tmpdrawcoordAux_y);
			if (tmpLength>length)
			{
				length=tmpLength;

				if(tmpdrawcoord_x<=tmpdrawcoordAux_x)
				{
					A=QPoint(tmpdrawcoord_x, tmpdrawcoord_y);
					B=QPoint(tmpdrawcoordAux_x, tmpdrawcoordAux_y);
				}
				else
				{
					A=QPoint(tmpdrawcoordAux_x, tmpdrawcoordAux_y);
					B=QPoint(tmpdrawcoord_x, tmpdrawcoord_y);
				}
			}

			tmpdrawcoordAux_x=tmpdrawcoord_x;
			tmpdrawcoordAux_y=tmpdrawcoord_y;
		}

		int wayWidth = DrawStyle::getMapDrawStyle()->getWayWidth(bbox_way->type, m_drawproperties->zoom);
		mapRegion->painter.setFont(QFont("Helvetica", qMin(wayWidth/2 + 2, 20)));

		int angle = wayinfo->calculateAngle(A, B);
		QFontMetrics fm = mapRegion->painter.fontMetrics();

		length = qSqrt(length);

		if(fm.width(info->name) + wayWidth <= length && fm.height() > 10)
		{
			if(angle)
			{
				QMatrix matrix;
				matrix.translate(A.x(), A.y());
				matrix.rotate(angle);

				mapRegion->painter.setMatrix(matrix);
				mapRegion->painter.drawText((length-fm.width(info->name))/2, fm.height()/4, info->name);
				mapRegion->painter.setMatrixEnabled(false);
			}
			else
				mapRegion->painter.drawText(A.x() + (length-fm.width(info->name))/2, A.y() + fm.height()/4, info->name);
		}

        if(suicide) return;

		m_tileServer->setBBox(bboxid);
	}
}

void MapImageDataServer::renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, MapRegion* mapRegion)
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::renderBBox";
#endif
    unsigned int bboxid = horiz_bbox_no  + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

    int horiz_coordoffset;
    if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
    else
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

    int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));

    // Check the tile server status
	m_tileServer->setBBox(bboxid);
	m_tileServer->setZoomLevel(zoom_level);

	if(m_tileServer->LoadBBox())
		return;

    // start with first way
    DT_OSMWayBBox* bbox_way = m_tileServer->getFirstWay();
    while (bbox_way != NULL)
    {
    	QPainterPath path;
        int draw_startx = 0, draw_starty = 0, tmpdrawcoord_x = 0, tmpdrawcoord_y = 0;
        int lastx = 0, lasty = 0;

        for (int i = 0; i < bbox_way->referencesNo; i++)
        {
            DT_BBoxNode *node = m_tileServer->getNode(bbox_way->referenceList[i]);

            // Error proof : normally we don't reach
            if (node == NULL) {
                break;
            }

            tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
            tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

            if (i == 0) {
            	lastx = draw_startx = tmpdrawcoord_x; lasty = draw_starty = tmpdrawcoord_y;
                path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
            }
            else if(i == bbox_way->referencesNo-1)
            	path.lineTo(tmpdrawcoord_x, tmpdrawcoord_y);
            else if(ABS(lastx-tmpdrawcoord_x)+ABS(lasty-tmpdrawcoord_y)>4)
			{
				lastx = tmpdrawcoord_x;
				lasty = tmpdrawcoord_y;
                path.lineTo(tmpdrawcoord_x, tmpdrawcoord_y);
			}
        }

        // Check if the way continues in other BBox
        if (bbox_way->nextBBoxId != 0) {
            int nextBboxId = bbox_way->nextBBoxId - 1;
            m_tileServer->setBBox(nextBboxId);
            m_tileServer->LoadBBox();
            int waycnt = m_tileServer->countWays(bbox_way->id, nextBboxId, bbox_way->zoomLevel);
            while (waycnt > 0) {
            	DT_OSMWayBBox* way = m_tileServer->getWay(bbox_way->id, nextBboxId, bbox_way->zoomLevel, waycnt - 1);
                if (way->prevBBoxNodeId == bbox_way->nextBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getNode(way->referenceList[0]);

                    int new_vert_bbox_no = nextBboxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = nextBboxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = ((node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE)) /m_drawproperties->res_x;
                    int coord_y = ((vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat)) /m_drawproperties->res_y;

                    path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
                    path.lineTo(coord_x, coord_y);

                    break;
                }
                waycnt--;
            }
        }

        if (bbox_way->prevBBoxId != 0) {
            int prevBBoxId = bbox_way->prevBBoxId - 1;
            m_tileServer->setBBox(prevBBoxId);
            m_tileServer->LoadBBox();
            int waycnt = m_tileServer->countWays(bbox_way->id, prevBBoxId, bbox_way->zoomLevel);
            while (waycnt > 0) {
            	DT_OSMWayBBox* way = m_tileServer->getWay(bbox_way->id, prevBBoxId, bbox_way->zoomLevel, waycnt - 1);
                if (way->nextBBoxNodeId == bbox_way->prevBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getNode(way->referenceList[way->referencesNo - 1]);

                    int new_vert_bbox_no = prevBBoxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = prevBBoxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = ((node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE)) /m_drawproperties->res_x;
                    int coord_y = ((vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat)) /m_drawproperties->res_y;

                    path.moveTo(draw_startx, draw_starty);
					path.lineTo(coord_x, coord_y);

					break;
                }
                waycnt--;
            }
        }

        if(suicide) return;

        mapRegion->painter.setPen(DrawStyle::getMapDrawStyle()->setRoadPenStyle(bbox_way->type, m_drawproperties->zoom, render_step));
        mapRegion->painter.drawPath(path);

        m_tileServer->setBBox(bboxid);
        bbox_way = m_tileServer->getNextWay();
    }
}

void MapImageDataServer::renderBBoxBoundaries (int horiz_bbox_no, int vert_bbox_no, MapRegion* mapRegion)
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::renderBBox";
#endif
    unsigned int bboxid = horiz_bbox_no  + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

    int horiz_coordoffset;
    if(mapRegion->marg.coord.long_start < mapinfo->getMapInfo()->left)
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE + CovertDecDeg2MapUnits(mapRegion->marg.coord.long_start, mapinfo->getMapInfo()->left);
    else
    	horiz_coordoffset = horiz_bbox_no * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, mapRegion->marg.coord.long_start);

    int vert_coordoffset = (mapRegion->marg.bbox.vert_size - (vert_bbox_no - mapRegion->marg.bbox.vert_start)) * BBOXSIZE -  ( (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, mapRegion->marg.coord.lat_stop));

    // Check the tile server status
	m_tileServer->setBBox(bboxid);
	m_tileServer->setZoomLevel(0);

	if(m_tileServer->LoadBBox())
		return;

    // start with first boundary
    DT_OSMBoundaryBBox* bbox_boundary = m_tileServer->getFirstBoundary();
    while (bbox_boundary != NULL)
    {
    	QPainterPath path;
        int draw_startx = 0, draw_starty = 0, tmpdrawcoord_x = 0, tmpdrawcoord_y = 0;

        for (int i = 0; i < bbox_boundary->referencesNo; i++)
        {
            DT_BBoxNode *node = m_tileServer->getNode(bbox_boundary->referenceList[i]);

            // Error proof : normally we don't reach
            if (node == NULL) {
                break;
            }

            tmpdrawcoord_x = ((node->bbox_long + horiz_coordoffset)) /m_drawproperties->res_x;
            tmpdrawcoord_y = ((vert_coordoffset - node->bbox_lat)) /m_drawproperties->res_y;

            if (i == 0) {
                draw_startx = tmpdrawcoord_x; draw_starty = tmpdrawcoord_y;
                path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
            }
            else
                path.lineTo(tmpdrawcoord_x, tmpdrawcoord_y);
        }

        // Check if the boundary continues in other BBox
        if (bbox_boundary->nextBBoxId != 0) {
            int nextBboxId = bbox_boundary->nextBBoxId - 1;
            m_tileServer->setBBox(nextBboxId);
            m_tileServer->LoadBBox();
            int boundarycnt = m_tileServer->countBoundaries(bbox_boundary->id, nextBboxId, 0);
            while (boundarycnt > 0) {
            	DT_OSMBoundaryBBox* boundary = m_tileServer->getBoundary(bbox_boundary->id, nextBboxId, 0, boundarycnt - 1);
                if (boundary->prevBBoxNodeId == bbox_boundary->nextBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getNode(boundary->referenceList[0]);

                    int new_vert_bbox_no = nextBboxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = nextBboxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = ((node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE)) /m_drawproperties->res_x;
                    int coord_y = ((vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat)) /m_drawproperties->res_y;

                    path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
                    path.lineTo(coord_x, coord_y);

                    break;
                }
                boundarycnt--;
            }
        }

        if (bbox_boundary->prevBBoxId != 0) {
            int prevBBoxId = bbox_boundary->prevBBoxId - 1;
            m_tileServer->setBBox(prevBBoxId);
            m_tileServer->LoadBBox();
            int boundarycnt = m_tileServer->countBoundaries(bbox_boundary->id, prevBBoxId, 0);
            while (boundarycnt > 0) {
            	DT_OSMBoundaryBBox* boundary = m_tileServer->getBoundary(bbox_boundary->id, prevBBoxId, 0, boundarycnt - 1);
                if (boundary->nextBBoxNodeId == bbox_boundary->prevBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getNode(boundary->referenceList[boundary->referencesNo - 1]);

                    int new_vert_bbox_no = prevBBoxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = prevBBoxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = ((node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE)) /m_drawproperties->res_x;
                    int coord_y = ((vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat)) /m_drawproperties->res_y;

                    path.moveTo(draw_startx, draw_starty);
					path.lineTo(coord_x, coord_y);

					break;
                }
                boundarycnt--;
            }
        }

        if(suicide) return;

        mapRegion->painter.drawPath(path);

        m_tileServer->setBBox(bboxid);
        bbox_boundary = m_tileServer->getNextBoundary();
    }
}

void MapImageDataServer::renderMapData(MapRegion* mapRegion)
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::renderMapData";
#endif

    int ii, jj;

	if(mapRegion->drawLevel < areasZoomLevel)
	{
		mapRegion->painter.setPen(Qt::transparent);

		for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
			for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
			{
				renderBBoxPolygons(ii, jj, mapRegion->drawLevel, mapRegion);

				if(suicide) return;
			}
	}
	else if(mapRegion->drawLevel < areasZoomLevel + waysZoomLevel)
	{
		mapRegion->painter.setBrush(Qt::NoBrush);

		for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
			for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
			{
				renderBBox(ii, jj, areasZoomLevel + waysZoomLevel - mapRegion->drawLevel-1, 0, mapRegion);

				if(suicide) return;
			}
	}
	else if(mapRegion->drawLevel < areasZoomLevel + 2*waysZoomLevel)
	{
		if (m_drawproperties->zoom < 2000)
			for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
				for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
				{
					renderBBox(ii, jj, areasZoomLevel + 2*waysZoomLevel - mapRegion->drawLevel-1, 1, mapRegion);

					if(suicide) return;
				}
	}
	else if(mapRegion->drawLevel == areasZoomLevel + 2*waysZoomLevel)
	{
		mapRegion->painter.setPen(QPen(QColor(0x8D44A5), 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));

		for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
			for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
			{
				renderBBoxBoundaries(ii, jj, mapRegion);

				if(suicide) return;
			}
	}
	else if(mapRegion->drawLevel == areasZoomLevel + 2*waysZoomLevel + 1)
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
		mapRegion->painter.setPen(QPen(Qt::black, 1));

		for(int zl=1; zl<placesLevel; zl++)
		{
			for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
				for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
				{
					drawPlaces(mapRegion, ii, jj, zl);

					if(suicide) return;
				}
		}
	}
	else if(mapRegion->drawLevel == areasZoomLevel + 2*waysZoomLevel + 2)
	{
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
		mapRegion->painter.setPen(Qt::black);

		for(int zl=0; zl<waysZoomLevel; zl++)
			for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
				for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
				{
					drawStreetName(mapRegion, ii, jj, zl);

					if(suicide) return;
				}
	}
	else
	{
		mapRegion->painter.setFont(QFont());
		for (jj = mapRegion->marg.bbox.vert_start; jj < (mapRegion->marg.bbox.vert_start + mapRegion->marg.bbox.vert_size); jj++)
			for (ii = mapRegion->marg.bbox.horiz_start; ii < (mapRegion->marg.bbox.horiz_start + mapRegion->marg.bbox.horiz_size) ; ii++)
			{
				drawPOIs(mapRegion, ii, jj);

				if(suicide) return;
			}

	}

	if(mapRegion->drawLevel==noDrawLevels-1 || mapRegion->drawLevel==areasZoomLevel + 2*waysZoomLevel-1)
	{
		m_painter->drawImage(mapRegion->startX, mapRegion->startY, mapRegion->image);
		emit dataReady();
	}
}

void MapImageDataServer::run ()
{
#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"MapImageDataServer::run";
#endif
    suicide=false;
    make_heap(regions.begin(), regions.end(), CompareRegions());

	waysZoomLevel = NO_ZOOMLEVELS - (m_drawproperties->zoom  / 3500);
	if (waysZoomLevel <= 1) waysZoomLevel = 2;
	if (waysZoomLevel >= NO_ZOOMLEVELS) waysZoomLevel = NO_ZOOMLEVELS;

	areasZoomLevel = NO_ZOOMLEVELS - (m_drawproperties->zoom  / 3500);
	if (areasZoomLevel <= 0) areasZoomLevel = 1;
	if (areasZoomLevel >= NO_ZOOMLEVELS) areasZoomLevel = NO_ZOOMLEVELS;

	placesLevel = 8;
	if(m_drawproperties->zoom >20000)
		placesLevel = 6;

	if(m_drawproperties->zoom > 80000)
		areasZoomLevel = 0;

	if(m_drawproperties->zoom > 100000)
		placesLevel = 5;

	if(m_drawproperties->zoom > 500000)
		placesLevel = 4;

	noDrawLevels = 2*waysZoomLevel + areasZoomLevel + 2;

	if(m_drawproperties->zoom < 400)
		noDrawLevels++;

	if(m_drawproperties->zoom < 250)
		noDrawLevels++;

    while(!suicide && !regions.empty())
    {
    	MapRegion *reg=regions.front();
    	pop_heap(regions.begin(), regions.end(), CompareRegions());
		regions.pop_back();

		MapRegion *reg2=regions.front();
		while(reg2 && reg2->drawLevel==reg->drawLevel && reg2->isVisible==reg->isVisible && reg->startX==reg2->startX && reg->width==reg2->width && reg->startY+reg->height==reg2->startY)
		{
			MapRegion *bigReg=new MapRegion(reg->startX, reg->startY, reg->width, reg->height+reg2->height, reg->drawLevel, reg->isVisible);
			bigReg->setMargins(reg->marg.coord.long_start, reg->marg.coord.long_stop, reg2->marg.coord.lat_start, reg->marg.coord.lat_stop);
			bigReg->painter.drawImage(0, 0, reg->image);
			bigReg->painter.drawImage(0, reg->height, reg2->image);

			pop_heap(regions.begin(), regions.end(), CompareRegions());
			regions.pop_back();

			delete reg;
			delete reg2;
			reg=bigReg;
			reg2=regions.front();
		}

		while(reg2 && reg2->drawLevel==reg->drawLevel && reg2->isVisible==reg->isVisible && reg->startY==reg2->startY && reg->height==reg2->height && reg->startX+reg->width==reg2->startX)
		{
			MapRegion *bigReg=new MapRegion(reg->startX, reg->startY, reg->width+reg2->width, reg->height, reg->drawLevel, reg->isVisible);
			bigReg->setMargins(reg->marg.coord.long_start, reg2->marg.coord.long_stop, reg->marg.coord.lat_start, reg->marg.coord.lat_stop);
			bigReg->painter.drawImage(0, 0, reg->image);
			bigReg->painter.drawImage(reg->width, 0, reg2->image);

			pop_heap(regions.begin(), regions.end(), CompareRegions());
			regions.pop_back();

			delete reg;
			delete reg2;
			reg=bigReg;
			reg2=regions.front();
		}

		renderMapData(reg);

    	if(!suicide)
			reg->drawLevel++;

		if(reg->drawLevel < noDrawLevels)
		{
			regions.push_back(reg);
			push_heap(regions.begin(), regions.end(), CompareRegions());
		}
		else
			delete reg;
    }

    if(!suicide)
    	m_tileServer->releaseData();
}
