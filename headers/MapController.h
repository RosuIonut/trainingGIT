/*
 * MapController.h
 *
 *  Created on: Aug 14, 2012
 *      Author: tudor.orha
 */

#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

#include <QObject>
#include <QList>

#include "MapDraw.h"

class MapController : public QObject
{
	Q_OBJECT

public:
	MapController(QObject *parent, MapDraw &map);
	~MapController();

	void addOverlay(QObject *overlay);
	void slide(int x, int y) {emit imageSlide(x, y);}

public slots:
	void onPositionChanged(double latitude, double longitude);
	void onPositionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude) {emit positionChanged(latitude, longitude);}
	void removeOverlay(QObject *overlay);

signals:
	void positionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude);
	void imageSlide(int x, int y);

private:
	MapDraw &m_map;
	QList<QObject*> m_overlays;

};

#endif /* MAPCONTROLLER_H_ */
