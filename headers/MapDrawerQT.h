#ifndef _MAPDRAW_H_
#define _MAPDRAW_H_

#include <MapDrawer.h>

#include "TileServer.h"
#include "Configs.h"

#include "MapImageDataServer.h"
#include "DataStructures.h"

#include <QtCore>
#include <QtGui>
#include <QtDebug>

class MapDrawerQT : public QWidget,  public MapDrawer
{
	Q_OBJECT

///////////////////////////////////////////////////////////////////////////////////////
public:
    MapDrawerQT(QWidget* parent, int width, int height);
    MapDrawerQT(QWidget* parent);
    ~MapDrawerQT();

///////////////////////////////////////////////////////////////////////////////////////
public slots:
    void  drawMap();

    void renderPath( QPainterPath path, QPen pen);
    void setBackground(QRect rectangle, QBrush brush);
    void drawLine(int x1, int y1, int x2, int y2, QPen pen);

    void  moveSlice(E_MoveDirection direction);
    void  zoomSlice (E_ZoomType zoom);
///////////////////////////////////////////////////////////////////////////////////////
signals:
	void slideMap (E_MoveDirection direction);
	void zoomMap (E_ZoomType zoom);
	void dataReady(void);

///////////////////////////////////////////////////////////////////////////////////////
private:
    void clear();
///////////////////////////////////////////////////////////////////////////////////////
private:

    // MapImageDataServer* mapdata;

     QImage	*	m_pimage;			// rendering image
     QPainter*  m_ppainter;			// paint device

     // Current draw margins as coordinates
     float long_start;
     float long_stop;
     float lat_start;
     float lat_stop;
     unsigned int clickLat,clickLong,moveLat,moveLong;
     int width, height;

///////////////////////////////////////////////////////////////////////////////////////
protected:
    void paintEvent (QPaintEvent *p_event);
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseMoveEvent(QMouseEvent *e );
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);

};

#endif

