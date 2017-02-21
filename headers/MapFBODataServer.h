#ifndef MAPFBODATASERVER_H
#define MAPFBODATASERVER_H


#include "TileServer.h"
#include "Configs.h"
#include "DataTypes.h"

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
#include <QGLFramebufferObject>


class MapFBODataServer : public QThread
{
    Q_OBJECT

public:

    ~MapFBODataServer();

    static MapFBODataServer *getMapImageDataServer(int width, int height);
    static MapFBODataServer *getMapImageDataServer();

    QImage	 getBufferImage () { return *m_pimage;}
    //QGLFramebufferObject  getFBO() {return  *render_fbo;}

    void loadMapSlice (float _long_start, float _long_stop, float _lat_start, float _lat_stop);
    void loadMapSlice (float _long_center, float _lat_center, float _zoom = 0);

    void renderMapData (DT_stDrawRegion draw_region, DT_stMapMarginsCoord draw_coord);
    void renderBBoxPolygons (int horiz_bbox_no, int vert_bbox_no, int zoom_level);

    float getMapSliceZoom() {return m_mapp.zoom;}
    DT_MapCoordinate getMapSliceCenter() {return m_mapp.centre;}
    DT_stMapMarginsCoord getMapSliceCoord() {return m_mapmarg.coord;}
    DT_MapInfo getMapCoord() {return *mapinfo->getMapInfo();}

public slots:
    void moveSlice(E_MoveDirection direction);
    void zoomSlice (E_ZoomType zoom);
    void moveSliceMouse(DT_stMapMarginsCoord coord);

signals:
    void dataReady(void);

private:

    MapFBODataServer(int width, int height);
    static MapFBODataServer *s_instance;

    QImage	*	m_pimage;			// rendering image
//    QGLFramebufferObject *render_fbo;
//    QGLFramebufferObject *texture_fbo;
    QPainter* m_ppainter;			// paint device

    CMapInfo* mapinfo;
    CTileServer* m_tileServer;
    DT_stMapSliceMargins m_mapmarg;
    DT_stMapSlicePoint m_mapp;
    DT_stDrawProperties m_drawproperties;

    void run();

    void calcDrawResolution();
    void calcDrawBBoxes(DT_stMapMarginsBBox& bbox_margins, DT_stMapMarginsCoord coord_margins);

    void renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, DT_stDrawRegion draw_region);

//protected:
};

#endif // MAPFBODATASERVER_H
