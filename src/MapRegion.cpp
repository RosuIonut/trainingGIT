#include "MapRegion.h"
#include "MapInfo.h"
#include "DrawStyle.h"

extern "C" {
#include "MapUtils.h"
}

MapRegion::MapRegion(const int startX, const int startY, const int width, const int height, const int drawLevel, const bool isVisible) :
	startX(startX), startY(startY), width(width), height(height), drawLevel(drawLevel), isVisible(isVisible), image(width, height, QImage::Format_RGB16)
{
	painter.begin(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(0, 0, width, height, DrawStyle::getMapDrawStyle()->getBackColor());
}

MapRegion::~MapRegion()
{

}

void MapRegion::setMargins(const DT_MapCoordinate x1, const DT_MapCoordinate x2, const DT_MapCoordinate y1, const DT_MapCoordinate y2)
{
	marg.coord.long_start=x1;
	marg.coord.long_stop=x2;
	marg.coord.lat_start=y1;
	marg.coord.lat_stop=y2;

	calcDrawBBoxes();
}

void MapRegion::calcDrawBBoxes()
{
    if(marg.coord.long_start < CMapInfo::instance()->getMapInfo()->left)
    	marg.bbox.horiz_start=0;
    else
    	marg.bbox.horiz_start = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(CMapInfo::instance()->getMapInfo()->left, marg.coord.long_start));

    if(marg.coord.lat_start < CMapInfo::instance()->getMapInfo()->down)
    	marg.bbox.vert_start=0;
    else
    	marg.bbox.vert_start = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(CMapInfo::instance()->getMapInfo()->down, marg.coord.lat_start));

    if (marg.bbox.horiz_start) marg.bbox.horiz_start--;
    if (marg.bbox.vert_start) marg.bbox.vert_start--;

    if(marg.coord.long_stop > CMapInfo::instance()->getMapInfo()->right)
    	marg.bbox.horiz_size = CMapInfo::instance()->getMapInfo()->horBBoxNo;
    else
    	marg.bbox.horiz_size = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(CMapInfo::instance()->getMapInfo()->left, marg.coord.long_stop));

    if(marg.coord.lat_stop > CMapInfo::instance()->getMapInfo()->up)
    	marg.bbox.vert_size = CMapInfo::instance()->getMapInfo()->verBBoxNo;
    else
    	marg.bbox.vert_size = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(CMapInfo::instance()->getMapInfo()->down, marg.coord.lat_stop));

    marg.bbox.horiz_size -= marg.bbox.horiz_start;
    marg.bbox.vert_size -= marg.bbox.vert_start;
}

