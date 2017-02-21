#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include "DataStructures.h"
#include "DataTypes.h"

#include "Configs.h" // DEGPRECISION

// qt
#include <QPen>
#include <QPainterPath>

extern "C" {
#include "MapInfo.h"
//#include "FastUtils.h"
#include "MapUtils.h"
//#include "StringParsers.h"
//#include "DataInfo.h"
//#include "DataStructures.h"
}

#include "TileServer.h"
#include "MapImageDataServer.h"
//class CTitleServer;

class MapDrawer
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
    MapDrawer();
    virtual ~MapDrawer();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
signals:
    void dataReady(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
public slots: // methods re-implemented by each drawer type in its own way
    virtual void  drawMap() = 0;
    ///
    virtual void  moveSlice(E_MoveDirection direction) = 0;
    //virtual void  zoomSlice (E_ZoomType zoom) = 0;

    /// these methods will be reimplemented by each drawer in it's own way in order to allow
    /// multimple drawing methods, following the same drawing algorithm
    virtual void renderPath(QPainterPath path, QPen pen) = 0;
    virtual void setBackground(QRect rectangle, QBrush brush) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2, QPen pen) = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
    void loadMapSlice(float _long_start, float _long_stop, float _lat_start, float _lat_stop);
    void calcDrawBBoxes(DT_stMapMarginsBBox& bbox_margins, DT_stMapMarginsCoord coord_margins);
    void calcDrawResolution ();

    // renderMapData and renderBBox methods are using the template design pattern
    void renderMapData(DT_stDrawRegion draw_region, DT_stMapMarginsCoord draw_coord);
    void renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, DT_stDrawRegion draw_region);

    void computeDrawingCoordinates(E_MoveDirection direction);
    void computeDrawingCoordinates(E_ZoomType zoom);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
    CMapInfo* mapinfo;                      // used by loadslice()
    DT_stMapSliceMargins m_mapmarg;         // used by loadslice()
    DT_stDrawProperties m_drawproperties;   // used by loadslice()
    DT_stMapSlicePoint m_mapp;              // used by calcDrawResolution()
    CTileServer* m_tileServer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    void Init();
};

#endif // MAPDRAWER_H
