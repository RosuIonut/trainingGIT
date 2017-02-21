#include "MapDrawerQT.h"
#include  "QDebug"

#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
# include <QTime>
typedef QTime QElapsedTimer;
#else
# include <QElapsedTimer>
#endif

//#define _DEBUG_MAP_DRAW_

MapDrawerQT::MapDrawerQT(QWidget* parent, int width, int height)
  :QWidget(parent), width(width), height(height)
 {

    resize(width,height);
    hide();

    #ifdef _MAP_DRAW_DEBUG_
        qDebug()<<"this->width()="<< this->width()<<" ; this->height()="<<this->height());
    #endif

    connect(this, SIGNAL(dataReady()), this, SLOT(drawMap()));
    connect(this, SIGNAL(slideMap(E_MoveDirection)), this, SLOT(moveSlice(E_MoveDirection)));
    connect(this, SIGNAL(zoomMap (E_ZoomType)), this, SLOT(zoomSlice(E_ZoomType)));

    //    long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
            //long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
    //        long_start = 24.313;	long_stop = 24.3796; lat_start = 46.1736; lat_stop = 46.1347; // medias
              long_start = 23.55981;	long_stop = 23.63354; lat_start = 46.75924; lat_stop = 46.78299; // cluj
    //        long_start = 24.51;	long_stop = 24.6; lat_start = 46.51; lat_stop = 46.56; //mures
        //    long_start = 25.50;	long_stop = 26.5; lat_start = 43.70; lat_stop = 44.70; //bucuresti
    //
            m_drawproperties.width = width; m_drawproperties.height = height;

            m_pimage = new QImage(m_drawproperties.width, m_drawproperties.height , QImage::Format_RGB32);
            m_ppainter = new QPainter(m_pimage);

            m_tileServer = NULL;
            mapinfo = CMapInfo::instance();

            //load map slice
            loadMapSlice(long_start, long_stop , lat_start , lat_stop);

            //start
            DT_stDrawRegion draw_region;
            draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
            draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

            renderMapData(draw_region, m_mapmarg.coord);

            emit dataReady();
        update();
        show();
        setFocus();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MapDrawerQT::~MapDrawerQT()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerQT::drawMap()
{
    update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerQT::paintEvent(QPaintEvent *p_event)
{
    QPainter painter(this);

    QElapsedTimer timmer;
    timmer.start();

    painter.drawImage(QPoint(0 , 0), *m_pimage);

    qDebug() << " Drawing with QT time elapsed = " << timmer.elapsed() << " miliseconds." << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerQT::renderPath(QPainterPath path, QPen pen)
{
    m_ppainter->setPen(pen);
    m_ppainter->drawPath(path);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::setBackground(QRect rectangle, QBrush brush)
{
    m_ppainter->fillRect(rectangle, brush);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::drawLine(int x1, int y1, int x2, int y2, QPen pen)
{
     m_ppainter->setPen(pen);
     m_ppainter->drawLine(x1,y1,x2,y2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerQT::moveSlice(E_MoveDirection direction)
{
    computeDrawingCoordinates(direction);

    // map coordinates
    DT_stMapMarginsCoord draw_coord;
    draw_coord.long_start = m_mapmarg.coord.long_start; draw_coord.long_stop = m_mapmarg.coord.long_stop;
    draw_coord.lat_start = m_mapmarg.coord.lat_start; draw_coord.lat_stop = m_mapmarg.coord.lat_stop;

    calcDrawBBoxes(m_mapmarg.bbox, draw_coord);

    // window coordinates
    DT_stDrawRegion draw_region;
    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

    clear();
    renderMapData(draw_region, draw_coord);

    emit dataReady();

#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"\n---------- MapImageDataServer.cpp::moveSlice ------------- \n";
    qDebug()<<"long_start="<< m_mapmarg.coord.long_start<<", lat_start="<< m_mapmarg.coord.lat_start<<", long_stop="<< m_mapmarg.coord.long_stop<<", lat_stop="<< m_mapmarg.coord.lat_stop<<"\n";
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerQT::zoomSlice (E_ZoomType zoom)
{
    computeDrawingCoordinates(zoom);

    calcDrawBBoxes(m_mapmarg.bbox, m_mapmarg.coord);

    calcDrawResolution();

    clear();

    DT_stDrawRegion draw_region;
    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

    renderMapData(draw_region, m_mapmarg.coord);
    emit dataReady();

#ifdef _DEBUG_MAPIMAGEDATASERVER_
    qDebug()<<"\n---------- MapImageDataServer.cpp::zoomSlice ------------- \n";
    qDebug()<<"long_start="<< m_mapmarg.coord.long_start<<", lat_start="<< m_mapmarg.coord.lat_start<<", long_stop="<< m_mapmarg.coord.long_stop<<", lat_stop="<< m_mapmarg.coord.lat_stop<<"\n";
#endif

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::clear()
{
   // if(m_pimage)
    {
   //   delete m_pimage;
      m_pimage = new QImage(m_drawproperties.width, m_drawproperties.height , QImage::Format_RGB32);
    }

   // if(m_ppainter)
    {
    //  delete m_ppainter;
      m_ppainter = new QPainter(m_pimage);
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::mousePressEvent ( QMouseEvent *event)
{
//    if (event->button() == Qt::LeftButton){
//        QPoint loc=event->pos();
//        clickLat = FixMapCoordDecDeg(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,(loc.y()*(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,mapdata->getMapSliceCoord().lat_start)))/height));
//        clickLong = FixMapCoordDecDeg(AddMapCoord(mapdata->getMapSliceCoord().long_start,(loc.x()*(DiffMapCoord(mapdata->getMapSliceCoord().long_stop,mapdata->getMapSliceCoord().long_start)))/width));

//        setMouseTracking(true);
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::mouseReleaseEvent ( QMouseEvent *event)
{
    setMouseTracking(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::mouseMoveEvent( QMouseEvent *event)
{
//    QPoint loc=event->pos();

//    DT_stMapMarginsCoord aux = mapdata->getMapSliceCoord();
//    DT_MapInfo mapMargins = mapdata->getMapCoord();

//    moveLat = FixMapCoordDecDeg(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,(loc.y()*(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,mapdata->getMapSliceCoord().lat_start)))/height));
//    moveLong = FixMapCoordDecDeg(AddMapCoord(mapdata->getMapSliceCoord().long_start,(loc.x()*(DiffMapCoord(mapdata->getMapSliceCoord().long_stop,mapdata->getMapSliceCoord().long_start)))/width));

//    unsigned int Latdiff = DifCoord(clickLat,moveLat);
//    unsigned int Longdiff = DifCoord(clickLong,moveLong);

//    if (clickLong >= moveLong)
//    {
//        if (mapMargins.left <= AddMapCoord(aux.long_start,Longdiff))
//            aux.long_start = AddMapCoord(aux.long_start,Longdiff);
//        if (mapMargins.right >=AddMapCoord(aux.long_stop,Longdiff))
//            aux.long_stop = AddMapCoord(aux.long_stop,Longdiff);
//    }
//     else
//     {
//        if (mapMargins.left <= AddMapCoord(aux.long_start,Longdiff))
//            aux.long_start = DiffMapCoord(aux.long_start,Longdiff);
//        if (mapMargins.right >=AddMapCoord(aux.long_stop,Longdiff))
//            aux.long_stop = DiffMapCoord(aux.long_stop,Longdiff);
//     }

//    if (clickLat >= moveLat)
//    {
//        if (mapMargins.up >= AddMapCoord(aux.lat_start,Latdiff))
//            aux.lat_start = AddMapCoord(aux.lat_start,Latdiff);
//        if (mapMargins.down <= AddMapCoord(aux.lat_stop,Latdiff))
//            aux.lat_stop = AddMapCoord(aux.lat_stop,Latdiff);
//    }

//    else
//    {
//         if (mapMargins.up >= AddMapCoord(aux.lat_start,Latdiff))
//            aux.lat_start = DiffMapCoord(aux.lat_start,Latdiff);
//         if (mapMargins.down <= AddMapCoord(aux.lat_stop,Latdiff))
//            aux.lat_stop = DiffMapCoord(aux.lat_stop,Latdiff);
//    }

//    mapdata->moveSliceMouse(aux);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerQT::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
		case Qt::Key_Left:
			emit(slideMap(E_Left));
			break;
		case Qt::Key_Right:
            emit(slideMap(E_Right));
			break;
		case Qt::Key_Down:
            emit(slideMap(E_Down));
            break;
		case Qt::Key_Up:
            emit(slideMap(E_Up));
			break;
		case Qt::Key_Return:
			break;
		case Qt::Key_Space:
			break;
		default:
			break;
    }
}

void MapDrawerQT::wheelEvent(QWheelEvent* event)
{
	if(event->delta() > 0) {
		emit(zoomMap(E_ZoomIn));
	}
	else {
		emit(zoomMap(E_ZoomOut));
	}
}



