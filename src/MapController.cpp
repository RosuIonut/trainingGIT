/*
 * MapController.cpp
 *
 *  Created on: Aug 14, 2012
 *      Author: tudor.orha
 */

#include <QDebug>

#include "MapController.h"
#include "MapUtils.h"

MapController::MapController(QObject *parent /*= 0*/, MapDraw &map)
:	QObject(parent)
,	m_map(map)
{
	connect(this, SIGNAL(positionChanged(DT_MapCoordinate, DT_MapCoordinate)), &m_map, SLOT(onPositionChanged(DT_MapCoordinate, DT_MapCoordinate)));
	connect(this, SIGNAL(imageSlide(int, int)), &m_map, SLOT(onImageSlide(int, int)));
}


MapController::~MapController()
{
	
	while (!m_overlays.isEmpty())
		removeOverlay(m_overlays.first());

	disconnect(this, SIGNAL(positionChanged(DT_MapCoordinate, DT_MapCoordinate)), &m_map, SLOT(onPositionChanged(DT_MapCoordinate, DT_MapCoordinate)));
	disconnect(this, SIGNAL(imageSlide(int, int)), &m_map, SLOT(onImageSlide(int, int)));
	
}

void MapController::addOverlay(QObject *overlay)
{
	
	if (overlay == 0)
		return;
	if (m_overlays.contains(overlay))
		return;

	connect(this, SIGNAL(positionChanged(DT_MapCoordinate, DT_MapCoordinate)), overlay, SLOT(onPositionChanged(DT_MapCoordinate, DT_MapCoordinate)));
	connect(this, SIGNAL(imageSlide(int, int)), overlay, SLOT(onImageSlide(int, int)));
	connect(&m_map, SIGNAL(propChanged(DT_stDrawProperties*&)), overlay, SLOT(onPropertiesChanged(DT_stDrawProperties*&)));

	connect(overlay, SIGNAL(destroyed(QObject*)), this, SLOT(removeOverlay(QObject*)));

	m_map.initialize();

	m_overlays.append(overlay);
	
}

void MapController::removeOverlay(QObject *overlay)
{
	if (m_overlays.removeAll(overlay) > 0) {
		disconnect(this, SIGNAL(positionChanged(DT_MapCoordinate, DT_MapCoordinate)), overlay, SLOT(onPositionChanged(DT_MapCoordinate, DT_MapCoordinate)));
		disconnect(this, SIGNAL(imageSlide(int, int)), overlay, SLOT(onImageSlide(int, int)));
		disconnect(&m_map, SIGNAL(propChanged(DT_stDrawProperties*&)), overlay, SLOT(onPropertiesChanged(DT_stDrawProperties*&)));
	}
}

void MapController::onPositionChanged(double latitude, double longitude)
{
	DT_MapCoordinate DTlatitude = DoubleToMapCoord(latitude), DTlongitude = DoubleToMapCoord(longitude);
	emit positionChanged(DTlatitude, DTlongitude);
}


