#ifndef MAPDRAWERGL_H
#define MAPDRAWERGL_H

#include "MapDrawer.h"

//#include "TileServer.h"
//#include "Configs.h"

//#include <QtCore>
//#include <QtGui>
//#include <QtDebug>
#include <QGLWidget>
#include <QtOpenGL>

#include "DataStructures.h"
#include "DataTypes.h"

//extern "C" {
//#include "MapInfo.h"
//#include "FastUtils.h"
//#include "MapUtils.h"
//#include "StringParsers.h"
//#include "DataInfo.h"
//#include "DataStructures.h"
//}

#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
# include <QTime>
typedef QTime QElapsedTimer;
#else
# include <QElapsedTimer>
#endif

class MapDrawerGL : public QGLWidget , public MapDrawer
{
    Q_OBJECT

public:
    MapDrawerGL(QWidget* parent, int width, int height);
    MapDrawerGL(QWidget* parent);
    virtual ~MapDrawerGL();

//////////////////////////////////////////////////////
public slots:
    void  initializeGL();
    void  paintGL();
    void  resizeGL(int width, int height);

    void  drawMap ();

    void  moveSlice (E_MoveDirection direction);
    void  zoomSlice (E_ZoomType zoom);

    void renderPath(QPainterPath path, QPen pen);
    void setBackground(QRect rectangle, QBrush brush);
    void drawLine(int x1, int y1, int x2, int y2, QPen pen) {};

signals:
    void slideMap (E_MoveDirection direction);
    void zoomMap (E_ZoomType zoom);
    void dataReady(void);

private:

    // Current draw margins as coordinates
    float long_start;
    float long_stop;
    float lat_start;
    float lat_stop;
    unsigned int clickLat,clickLong,moveLat,moveLong;
    int width, height;

    QGLFramebufferObject *render_fbo;
    QGLFramebufferObject *texture_fbo;
    GLuint tile_list;

    QPainter* m_ppainter;
    QImage* m_pimage;

    //CMapInfo* mapinfo;
   // CTileServer* m_tileServer;
   // DT_stMapSliceMargins m_mapmarg;
   // DT_stMapSlicePoint m_mapp;
   // DT_stDrawProperties m_drawproperties;

//    CMapInfo* mapinfo;                      // used by loadslice()
//    DT_stMapSliceMargins m_mapmarg;         // used by loadslice()
//    DT_stDrawProperties m_drawproperties;   // used by loadslice()
//    DT_stMapSlicePoint m_mapp;              // used by calcDrawResolution()

protected:
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent(QMouseEvent *e );
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    void saveGLState();
    void restoreGLState();
    void draw();
    void clear();
  //  void loadMapSlice(float _long_start, float _long_stop, float _lat_start, float _lat_stop);
  //  void calcDrawBBoxes(DT_stMapMarginsBBox& bbox_margins, DT_stMapMarginsCoord coord_margins);
  //  void calcDrawResolution ();
 //   void renderMapData (DT_stDrawRegion draw_region, DT_stMapMarginsCoord draw_coord);
 //   void renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, DT_stDrawRegion draw_region);

};

#endif
