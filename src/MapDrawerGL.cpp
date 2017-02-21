#include "MapDrawerGL.h"
#include "DrawStyle.h"

#include  "QDebug"

MapDrawerGL::MapDrawerGL(QWidget* parent, int width, int height)
   :QGLWidget(QGLFormat(QGL::SampleBuffers), parent), width(width), height(height)
   {
   resize(width,height);
   hide();

    #ifdef _MAP_DRAW_DEBUG_
        qDebug()<<"this->width()="<< this->width()<<" ; this->height()="<<this->height());
    #endif

    makeCurrent();

    // create frame buffers
    if (QGLFramebufferObject::hasOpenGLFramebufferBlit()) {
            QGLFramebufferObjectFormat format;
            //format.setSamples(4);
            format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

            render_fbo = new QGLFramebufferObject(512, 512, format);
            texture_fbo = new QGLFramebufferObject(512, 512);

    } else {
            render_fbo = new QGLFramebufferObject(1024, 1024);
            texture_fbo = render_fbo;
    }

    m_ppainter = new QPainter(render_fbo);

    connect(this, SIGNAL(dataReady()), this, SLOT(drawMap()));
    connect(this, SIGNAL(slideMap(E_MoveDirection)), this, SLOT(moveSlice(E_MoveDirection)));
    connect(this, SIGNAL(zoomMap (E_ZoomType)), this, SLOT(zoomSlice(E_ZoomType)));

//        long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
//        long_start = 24.163837;	long_stop = 24.303837; lat_start = 46.065534; lat_stop = 46.105534;
//        long_start = 24.313;	long_stop = 24.3796; lat_start = 46.1736; lat_stop = 46.1347; // medias
          long_start = 23.55981;	long_stop = 23.63354; lat_start = 46.75924; lat_stop = 46.78299; //cluj
//        long_start = 24.51;	long_stop = 24.6; lat_start = 46.51; lat_stop = 46.56; //mures
//        long_start = 25.50;	long_stop = 26.5; lat_start = 43.70; lat_stop = 44.70; //bucuresti


      m_drawproperties.width = width; m_drawproperties.height = height;

      // m_pimage = new QImage(m_drawproperties.width, m_drawproperties.height , QImage::Format_RGB32);
      // m_ppainter = new QPainter(m_pimage);

      m_tileServer = NULL;
      mapinfo = CMapInfo::instance();

      //load map slice
      loadMapSlice(long_start, long_stop , lat_start , lat_stop);

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
MapDrawerGL::~MapDrawerGL()
{
    if(render_fbo) delete render_fbo;
    if(texture_fbo) delete texture_fbo;

    if(m_ppainter) delete m_ppainter;
    if(m_pimage) delete m_pimage;

    if(mapinfo) delete mapinfo;
    if(m_tileServer) delete m_tileServer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerGL::drawMap()
{
    updateGL();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::initializeGL()
{
    tile_list = glGenLists(1);
    glNewList(tile_list, GL_COMPILE);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    }
    glEnd();
    glEndList();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::resizeGL(int width, int height)
{
        if(height == 0) height = 1;

        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0f, (GLfloat) width, (GLfloat) height, 0.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This method renders in a opengl texture buffer and and displays the texture on a GLwidget
void MapDrawerGL::paintGL()
{
        QElapsedTimer timmer;
        timmer.start();

        draw();

        qDebug() << "Drawing with openGL time elapsed = " << timmer.elapsed() << " miliseconds." << endl;

        return;

        // Alternatively one can render as usually to QImage and then send the QImage to openGL for rendering.
        // in order to do this one should instantiate the  m_ppainter as  m_ppainter = new QPainter(m_pimage)
        // and use insted of draw() method the following lines of code:
        // QPainter p(this);
        // QImage renderImage = QGLWidget::convertToGLFormat(*m_pimage);
        // glDrawPixels(renderImage.width(), renderImage.height(), GL_RGBA, GL_UNSIGNED_BYTE, renderImage.bits());


//        // test purpose only
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER);

//        glPushMatrix();
//            glBegin(GL_LINES);
//                glColor3f(1,0,0);
//                glVertex2f(0,0.1);
//                glVertex2f(0.1,0.1);
//                glVertex2f(0.1,0);
//            glEnd();
//        glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::draw()
{
    QPainter p(this);

    // save the GL state set for QPainter
    saveGLState();

    if (render_fbo != texture_fbo) {
        QRect rect(0, 0, render_fbo->width(), render_fbo->height());
        QGLFramebufferObject::blitFramebuffer(texture_fbo, rect, render_fbo, rect);

//       QImage catchImage = render_fbo->toImage();
//       catchImage.save("uffa.png");
    }

    p.beginNativePainting();

    // draw into the GL widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); //
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 10, 100);
    glTranslatef(0.0f, 0.0f, -15.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture_fbo->texture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    glPushMatrix();
    glScalef(1.7f, 1.7f, 1.7f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glCallList(tile_list);
    glPopMatrix();

    p.endNativePainting();

    render_fbo->release();
    texture_fbo->release();
    glDisable(GL_MULTISAMPLE);

    // restore the GL state that QPainter expects
    restoreGLState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  MapDrawerGL::renderPath(QPainterPath path, QPen pen)
{
    m_ppainter->setPen(pen);
    m_ppainter->drawPath(path);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::setBackground(QRect rectangle, QBrush brush)
{
     m_ppainter->fillRect(rectangle, brush);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::saveGLState()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::restoreGLState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::mousePressEvent ( QMouseEvent *event)
{
//    if (event->button() == Qt::LeftButton){
//        QPoint loc=event->pos();
//        clickLat = FixMapCoordDecDeg(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,(loc.y()*(DiffMapCoord(mapdata->getMapSliceCoord().lat_stop,mapdata->getMapSliceCoord().lat_start)))/height));
//        clickLong = FixMapCoordDecDeg(AddMapCoord(mapdata->getMapSliceCoord().long_start,(loc.x()*(DiffMapCoord(mapdata->getMapSliceCoord().long_stop,mapdata->getMapSliceCoord().long_start)))/width));

//        setMouseTracking(true);
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::mouseReleaseEvent ( QMouseEvent *event)
{
    setMouseTracking(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::mouseMoveEvent( QMouseEvent *event)
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::keyPressEvent(QKeyEvent *event)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::wheelEvent(QWheelEvent* event)
{
    if(event->delta() > 0) {
        emit(zoomMap(E_ZoomIn));
    }
    else {
        emit(zoomMap(E_ZoomOut));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::moveSlice (E_MoveDirection direction)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::clear()
{
    if (QGLFramebufferObject::hasOpenGLFramebufferBlit())
    {
            QGLFramebufferObjectFormat format;
            format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

            render_fbo = new QGLFramebufferObject(512, 512, format);
            texture_fbo = new QGLFramebufferObject(512, 512);

    } else
    {
            render_fbo = new QGLFramebufferObject(1024, 1024);
            texture_fbo = render_fbo;
    }

    m_ppainter = new QPainter(render_fbo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MapDrawerGL::zoomSlice (E_ZoomType zoom)
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















