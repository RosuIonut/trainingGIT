#ifndef _DATATYPES_H_
#define _DATATYPES_H_

extern "C" {
#include "DataStructures.h"
}

typedef struct
{
	int horiz_start;
	int vert_start;
	int horiz_size;
	int vert_size;
}DT_stMapMarginsBBox;

typedef struct
{
	DT_MapCoordinate long_start;
	DT_MapCoordinate long_stop;
	DT_MapCoordinate lat_start;
	DT_MapCoordinate lat_stop;
}DT_stMapMarginsCoord;

typedef struct
{
	DT_stMapMarginsBBox bbox;
	DT_stMapMarginsCoord coord;
}DT_stMapSliceMargins;

typedef struct
{
	double res_x, res_y;
//	int resPrecision;
	double res_xy;
	DT_MapUnit zoom;

	int bufferWidth, bufferHeight;

	int startX, startY, width, height;

	DT_stMapMarginsCoord visibleMarg;
	DT_stMapMarginsCoord bufferMarg;
}DT_stDrawProperties;

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
}DT_stDrawRegion;

typedef struct
{
	DT_MapCoordinate centre;
	DT_MapUnit zoom;
}DT_stMapSlicePoint;

enum E_BboxConnType {
    E_NextBBoxConnection,
    E_PrevBBoxConnection
};
enum E_ZoomType {
    E_ZoomIn,
    E_ZoomOut
};
enum E_MoveDirection {
    E_Left,
    E_Right,
    E_Down,
    E_Up
};

#endif //_DATATYPES_H_
