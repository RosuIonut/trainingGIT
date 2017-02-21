#include "MapDrawerGS.h"
#include "DrawStyle.h"

#include  "QDebug"
#include <QtAlgorithms>

MapDrawerGS::MapDrawerGS(QWidget* parent, int width, int height)
   :QGLWidget(QGLFormat(QGL::SampleBuffers), parent), width(width), height(height)
   {
    resize(width,height);
    hide();

    //-----------------------------------------------------------------------------------//
    mGraphicsView = new QGraphicsView(this);
    mGraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    mGraphicsScene = new QGraphicsScene(0, 0, width, height, this);

    // setup the view
    mGraphicsView->setScene(mGraphicsScene);
    mGraphicsView->show();
    mGraphicsView->setSceneRect(0, 0, 5000, 5000);

    SetCenter(QPointF(0.0, 0.0));
    mGraphicsView->setCursor(Qt::OpenHandCursor);

    setMouseTracking(true);

    #ifdef _MAP_DRAW_DEBUG_
        qDebug()<<"this->width()="<< this->width()<<" ; this->height()="<<this->height());
    #endif

    connect(this, SIGNAL(dataReady()), this, SLOT(drawMap()));
    connect(this, SIGNAL(slideMap(E_MoveDirection)), this, SLOT(moveSlice(E_MoveDirection)));
 //   connect(this, SIGNAL(zoomMap (E_ZoomType)), this, SLOT(zoomSlice(E_ZoomType)));

//        long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
//        long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
//        long_start = 24.313;	long_stop = 24.3796; lat_start = 46.1736; lat_stop = 46.1347; // medias
          long_start = 23.55981;	long_stop = 23.63354; lat_start = 46.75924; lat_stop = 46.78299; //cluj
//        long_start = 24.51;	long_stop = 24.6; lat_start = 46.51; lat_stop = 46.56; //mures
//        long_start = 25.50;	long_stop = 26.5; lat_start = 43.70; lat_stop = 44.70; //bucuresti


     m_drawproperties.width = width; m_drawproperties.height = height;

     m_tileServer = NULL;
     mapinfo = CMapInfo::instance();

     //load map slice
     loadMapSlice(long_start, long_stop, lat_start , lat_stop);

     //start
     DT_stDrawRegion draw_region;
     draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
     draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

     renderMapData(draw_region, m_mapmarg.coord);

     update();
     show();
     setFocus();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MapDrawerGS::~MapDrawerGS()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::mousePressEvent(QMouseEvent* event) {

    LastPanPoint = event->pos();
    mGraphicsView->setCursor(Qt::ClosedHandCursor);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::mouseReleaseEvent(QMouseEvent* event) {

    mGraphicsView->setCursor(Qt::OpenHandCursor);
    LastPanPoint = QPoint();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::mouseMoveEvent(QMouseEvent* event)
{
    if(!LastPanPoint.isNull()) {
        // Get how much we panned
        QPointF delta = mGraphicsView->mapToScene(LastPanPoint) - mGraphicsView->mapToScene(event->pos());
        LastPanPoint = event->pos();

        // Update the center ie. do the pan
        SetCenter(GetCenter() + delta);
    }
    else
    {
        qDebug() << "mouseMoveEvent, but last point is null" << endl;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::keyPressEvent(QKeyEvent *event)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::SetCenter(const QPointF& centerPoint)
{
    QRectF visibleArea =  mGraphicsView->mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = mGraphicsView->sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

        // The max boundary that the centerPoint can be to
        QRectF bounds(boundX, boundY, boundWidth, boundHeight);
        // tarom
        if(bounds.contains(centerPoint)) {
            //We are within the bounds
            CurrentCenterPoint = centerPoint;
        } else {
            //We need to clamp or use the center of the screen
            if(visibleArea.contains(sceneBounds)) {
                //Use the center of scene ie. we can see the whole scene
                CurrentCenterPoint = sceneBounds.center();
            } else {

                CurrentCenterPoint = centerPoint;

                // We need to clamp the center. The centerPoint is too large
                if(centerPoint.x() > bounds.x() + bounds.width()) {
                    CurrentCenterPoint.setX(bounds.x() + bounds.width());
                } else if(centerPoint.x() < bounds.x()) {
                    CurrentCenterPoint.setX(bounds.x());
                }

                if(centerPoint.y() > bounds.y() + bounds.height()) {
                    CurrentCenterPoint.setY(bounds.y() + bounds.height());
                } else if(centerPoint.y() < bounds.y()) {
                    CurrentCenterPoint.setY(bounds.y());
                }
            }
        }

        //Update the scrollbars
        mGraphicsView->centerOn(CurrentCenterPoint);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::initializeGL()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::resizeGL(int width, int height)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::paintGL()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::renderPath(QPainterPath path, QPen pen)
{
    mGraphicsScene->addPath(path, pen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::setBackground(QRect rectangle, QBrush brush)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::drawLine(int x1, int y1, int x2, int y2, QPen pen)
{
    mGraphicsScene->addLine(x1, y1, x2, y2, pen);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::drawMap()
{
   updateGL();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void MapDrawerGS::mousePressEvent ( QMouseEvent *event)
//{
////    if (event->button() == Qt::LeftButton){
////        QPoint loc=event->pos();
////        clickLat = FixMapCoordDecDeg(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,(loc.y()*(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,mapdata->getMapSliceCoord().lat_start)))/height));
////        clickLong = FixMapCoordDecDeg(AddMapCoord(mapdata->getMapSliceCoord().long_start,(loc.x()*(DiffMapCoord(mapdata->getMapSliceCoord().long_stop,mapdata->getMapSliceCoord().long_start)))/width));

////        setMouseTracking(true);
////    }
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void MapDrawerGS::mouseReleaseEvent ( QMouseEvent *event)
//{
//    setMouseTracking(false);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void MapDrawerGS::mouseMoveEvent( QMouseEvent *event)
//{
//    //if(event->modifiers() & Qt::ShiftModifier)
//    {
//           QPointF p(1.0,1.0); // mapToScene(event->pos());
//           mGraphicsView->translate( p.x(), p.y() );
//           mGraphicsView->centerOn(p);
//           qDebug() << " mouseMoveEvent  " << endl;
//    }


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

//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// zoom in / zoom out using graphicsView-scale method
void MapDrawerGS::wheelEvent(QWheelEvent* event)
{
       // Get the position of the mouse before scaling, in scene coords
        QPointF pointBeforeScale(mGraphicsView->mapToScene(event->pos()));

        //Get the original screen centerpoint
        QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

        //Scale the view ie. do the zoom
        double scaleFactor = 1.15; //How fast we zoom
        if(event->delta() > 0) {
            //Zoom in
            mGraphicsView->scale(scaleFactor, scaleFactor);
        } else {
            //Zooming out
            mGraphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }

        //Get the position after scaling, in scene coords
        QPointF pointAfterScale(mGraphicsView->mapToScene(event->pos()));

        //Get the offset of how the screen moved
        QPointF offset = pointBeforeScale - pointAfterScale;

        //Adjust to the new center for correct zooming
        QPointF newCenter = screenCenter + offset;
        SetCenter(newCenter);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGS::moveSlice (E_MoveDirection direction)
{
    float delta = 10;

    QPointF step(0.0, 0.0);
    QRectF visibleArea;

    switch (direction)
 {
    case E_Left:

        step.setX(-delta); step.setY(0);
        break;
    case E_Right:

        step.setX(delta); step.setY(0);
        break;
    case E_Down:

        step.setX(0); step.setY(delta);
        break;
    case E_Up:

        step.setX(0); step.setY(-delta);
        break;
    default:

     // delete all items and draw another slice again
     qDeleteAll( mGraphicsScene->items() );

     long_start = 24.51;	long_stop = 24.6; lat_start = 46.51; lat_stop = 46.56; //mures
     loadMapSlice(long_start, long_stop, lat_start , lat_stop);

     //start
     DT_stDrawRegion draw_region;
     draw_region.x1 = GetCenter().x(); draw_region.x2 = m_drawproperties.width;
     draw_region.y1 = GetCenter().y(); draw_region.y2 = m_drawproperties.height;

     renderMapData(draw_region, m_mapmarg.coord);

     break;
  }

  SetCenter(GetCenter() + step);

  return;
}




