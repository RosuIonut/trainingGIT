#ifndef MAPREGION_H_
#define MAPREGION_H_
#include <QRegion>
#include "DataTypes.h"
#include <QImage>
#include <QPainter>

class MapRegion
{
public:
	int startX, startY;
	int width, height;
	DT_stMapSliceMargins marg;

	int drawLevel;
	bool isVisible;

	QImage image;
	QPainter painter;

public:
	MapRegion(const int, const int, const int, const int, const int, const bool);
	~MapRegion();
	void setMargins(const DT_MapCoordinate, const DT_MapCoordinate, const DT_MapCoordinate, const DT_MapCoordinate);
	void calcDrawBBoxes();
};

struct CompareRegions : public std::binary_function<MapRegion*, MapRegion*, bool>
{
    bool operator()(const MapRegion* r1, const MapRegion* r2) const
    {
    	if(r1->isVisible==r2->isVisible)
    		if(r1->drawLevel==r2->drawLevel)
    			if(r1->startX==r2->startX)
    				return r1->startY>r2->startY;
    			else
    				return r1->startX>r2->startX;
    		else
    			return r1->drawLevel>r2->drawLevel;
		else
			return r1->isVisible<r2->isVisible;
    }
};

#endif /* MAPREGION_H_ */
