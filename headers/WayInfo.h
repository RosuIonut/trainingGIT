/*
 * WayInfo.h
 *
 *  Created on: Aug 21, 2012
 *      Author: Ionut Rosu
 */

#ifndef WAYINFO_H_
#define WAYINFO_H_

#include "DataStructures.h"
#include "TileServer.h"
#include "MapInfo.h"
#include "MapRegion.h"
#include "Configs.h"

extern "C" {
#include "MapUtils.h"
}


#include "QPainterPath"
#include "QDebug"
#include <QtCore>
#include <QtGui>

class CWayInfo
{
private:
	float angleValues[90];

public:
	CWayInfo();
	~CWayInfo();

	int calculateAngle(QPointF pointA,QPointF pointB);
	int angleValue(qreal elem,int start,int stop);
};


#endif /* WAYINFO_H_ */
