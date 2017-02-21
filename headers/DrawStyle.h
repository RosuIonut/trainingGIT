#ifndef _DRAWSTYLE_H_
#define _DRAWSTYLE_H_

extern "C" {
#include "MapInfo.h"
#include "FastUtils.h"
#include "MapUtils.h"
#include "StringParsers.h"
#include "DataInfo.h"
}

#include "Configs.h"

#include <QtGui>
#include <QtDebug>

class DrawStyle
{

public:
	~DrawStyle();

	static DrawStyle *getMapDrawStyle();

	QPen setRoadPenStyle (unsigned short way_type, unsigned int zoom, int render_step);
	QBrush setAreaBrushStyle (unsigned char area_type);
	QBrush getBackColor ();
	QString getPOIIcon(int,int);
	int getWayWidth(unsigned short way_type, unsigned int zoom);
	QColor getPOIColor(unsigned int type);
	int getPlaceSize(unsigned int);

private:
	DrawStyle();
	static DrawStyle *s_instance;

protected:

};

#endif //_DRAWSTYLE_H_
