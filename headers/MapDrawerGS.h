#ifndef MAPDRAWERGS_H
#define MAPDRAWERGS_H

#include <MapDrawer.h>

#include "TileServer.h"
#include "Configs.h"

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QGLWidget>
#include <QtOpenGL>

#include "DataStructures.h"
#include "DataTypes.h"

extern "C" {
#include "MapInfo.h"
#include "FastUtils.h"
#include "MapUtils.h"
#include "StringParsers.h"
#include "DataInfo.h"
#include "DataStructures.h"
}

//class MapFBODataServer;

#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
# include <QTime>
typedef QTime QElapsedTimer;
#else
# include <QElapsedTimer>
#endif

class MapDrawerGS : public  QGLWidget,  public MapDrawer
{
    Q_OBJECT

public:
    MapDrawerGS(QWidget* parent, int width, int height);
    MapDrawerGS(QWidget* parent);
    virtual ~MapDrawerGS();

//////////////////////////////////////////////////////
public slots:
    void  initializeGL();
    void  paintGL();
    void  resizeGL(int width, int height);

    // overridden methods
    void  drawMap ();
    void  moveSlice (E_MoveDirection direction);
    // void  zoomSlice (E_ZoomType zoom);

    // these methods are called from the inherited renderData method
    void renderPath(QPainterPath path, QPen pen);
    void setBackground(QRect rectangle, QBrush brush);
    void drawLine(int x1, int y1, int x2, int y2, QPen pen);

signals:
    void slideMap (E_MoveDirection direction);
   // void zoomMap (E_ZoomType zoom);
    void dataReady(void);

private:

    // Current draw margins as coordinates
    float long_start;
    float long_stop;
    float lat_start;
    float lat_stop;
    unsigned int clickLat,clickLong,moveLat,moveLong;
    int width, height;

    //Holds the current centerpoint for the view, used for panning and zooming
    QPointF CurrentCenterPoint;

    //From panning the view
    QPoint LastPanPoint;

    QGraphicsScene* mGraphicsScene;
    QGraphicsView*  mGraphicsView;

protected:
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent(QMouseEvent *e );
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    ///
    void SetCenter(const QPointF& centerPoint);
    QPointF GetCenter() { return CurrentCenterPoint; }

    void draw();
   // void loadMapSlice(float _long_start, float _long_stop, float _lat_start, float _lat_stop);
//    void calcDrawBBoxes(DT_stMapMarginsBBox& bbox_margins, DT_stMapMarginsCoord coord_margins);
//    void calcDrawResolution ();
//    void renderMapData (DT_stDrawRegion draw_region, DT_stMapMarginsCoord draw_coord);
//    void renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, DT_stDrawRegion draw_region);

};

#endif

// http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
