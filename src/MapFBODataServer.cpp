#include "MapFBODataServer.h"
#include "DrawStyle.h"
#include <QGLWidget>

//#define _DEBUG_MapFBODataServer_
MapFBODataServer* MapFBODataServer::s_instance = NULL;

MapFBODataServer::MapFBODataServer(int width, int height)
{
    m_drawproperties.width = width; m_drawproperties.height = height;

    m_pimage = new QImage(m_drawproperties.width, m_drawproperties.height , QImage::Format_RGB32);
    m_ppainter = new QPainter(m_pimage);


    m_tileServer = NULL;
    mapinfo = CMapInfo::instance();

    //-------------------------------------------------------------//
    //create frame buffer object
//    if (QGLFramebufferObject::hasOpenGLFramebufferBlit()) {
//            QGLFramebufferObjectFormat format;
//            // format.setSamples(4);
//            format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

//            render_fbo = new QGLFramebufferObject(m_drawproperties.width, m_drawproperties.height, format);
//            texture_fbo = new QGLFramebufferObject(m_drawproperties.width, m_drawproperties.height);
//    } else {
//            render_fbo = new QGLFramebufferObject(m_drawproperties.width, m_drawproperties.height);
//            texture_fbo = render_fbo;
//    }

//    fbo_painter = new QPainter(render_fbo);
    //-------------------------------------------------------------//
}

MapFBODataServer::~MapFBODataServer()
{
    delete m_tileServer;
    delete m_ppainter;
    delete m_pimage;
    delete s_instance;
}

MapFBODataServer *
MapFBODataServer::getMapImageDataServer(int width, int height)
{
    if (s_instance == NULL) {
        s_instance = new MapFBODataServer(width, height);
    }
    return s_instance;
}

MapFBODataServer *
MapFBODataServer::getMapImageDataServer()
{
    return s_instance;
}

//void MapFBODataServer::saveGLState()
//{
//    glPushAttrib(GL_ALL_ATTRIB_BITS);
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//}
//
//void MapFBODataServer::restoreGLState()
//{
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
//    glPopAttrib();
//}

void MapFBODataServer::calcDrawResolution ()
{
    // recalculate resolution
    long int start_x = (CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, m_mapmarg.coord.long_start) - m_mapmarg.bbox.horiz_start * BBOXSIZE);
    long int start_y = ((m_mapmarg.bbox.vert_start + m_mapmarg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, m_mapmarg.coord.lat_stop));
    long int stop_x = m_mapmarg.bbox.horiz_size * BBOXSIZE - ( (m_mapmarg.bbox.horiz_start + m_mapmarg.bbox.horiz_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, m_mapmarg.coord.long_stop));
    long int stop_y = m_mapmarg.bbox.vert_size * BBOXSIZE - (CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, m_mapmarg.coord.lat_start) - m_mapmarg.bbox.vert_start * BBOXSIZE);

    m_drawproperties.res_x = ((float)(stop_x - start_x) / m_drawproperties.width);
    m_drawproperties.res_y =  ((float)(stop_y - start_y) / m_drawproperties.height);

    // quick formula to compute the zoom
    m_mapp.zoom = (stop_x - start_x + stop_y - start_y) / 100;

#ifdef _DEBUG_MapFBODataServer_
    qDebug()<<"m_mapp.zoom:"<<m_mapp.zoom;
    qDebug()<<"m_drawproperties.res_x : "<< m_drawproperties.res_x<<"; m_drawproperties.res_y :"<< m_drawproperties.res_y;
#endif
}

void MapFBODataServer::calcDrawBBoxes(DT_stMapMarginsBBox& bbox_margins, DT_stMapMarginsCoord coord_margins)
{
    bbox_margins.horiz_start = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, coord_margins.long_start));
    bbox_margins.vert_start = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, coord_margins.lat_start));
    if (bbox_margins.horiz_start)   bbox_margins.horiz_start--;
    if (bbox_margins.vert_start)        bbox_margins.vert_start--;

    bbox_margins.horiz_size = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, coord_margins.long_stop));
    bbox_margins.vert_size = CalculateBBoxesNo(0, CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, coord_margins.lat_stop));
    bbox_margins.horiz_size -= bbox_margins.horiz_start;
    bbox_margins.vert_size -= bbox_margins.vert_start;

#ifdef _DEBUG_MapFBODataServer_
    qDebug()<<"bbox_margins.horiz_start : "<< bbox_margins.horiz_start<<"; bbox_margins.vert_start :"<< bbox_margins.vert_start<<"; horiz_bbox_size :"<< bbox_margins.horiz_size<<"; vert_bbox_size :"<< bbox_margins.vert_size<<"\n";
#endif
}

void MapFBODataServer::loadMapSlice (float _long_start, float _long_stop, float _lat_start, float _lat_stop)
{
    int deg_losta, deg_losto, deg_lasta, deg_lasto;
    float decdeg_losta, decdeg_losto, decdeg_lasta, decdeg_lasto;

    deg_losta = (int)_long_start; deg_losto = (int)_long_stop;      deg_lasta = (int)_lat_start; deg_lasto = (int)_lat_stop;
    decdeg_losta = _long_start - deg_losta; decdeg_losto = _long_stop - deg_losto; decdeg_lasta = _lat_start - deg_lasta; decdeg_lasto = _lat_stop - deg_lasto;
    deg_losta +=180; deg_losto += 180; deg_lasta += 180; deg_lasto += 180;

    int i = 0;
    while(i < DECDEGPRECISION) {
        decdeg_losta *= 10; decdeg_losto *= 10; decdeg_lasta *= 10; decdeg_lasto *= 10;
        ++i;
    }

    MAP_DEG2COORD(deg_losta, m_mapmarg.coord.long_start); MAP_DECDEG2COORD((int)decdeg_losta, m_mapmarg.coord.long_start);
    MAP_DEG2COORD(deg_losto, m_mapmarg.coord.long_stop); MAP_DECDEG2COORD((int)decdeg_losto, m_mapmarg.coord.long_stop);
    MAP_DEG2COORD(deg_lasta, m_mapmarg.coord.lat_start); MAP_DECDEG2COORD((int)decdeg_lasta, m_mapmarg.coord.lat_start);
    MAP_DEG2COORD(deg_lasto, m_mapmarg.coord.lat_stop); MAP_DECDEG2COORD((int)decdeg_lasto, m_mapmarg.coord.lat_stop);

    m_mapmarg.coord.long_start = m_mapmarg.coord.long_start > mapinfo->getMapInfo()->left ?  m_mapmarg.coord.long_start : mapinfo->getMapInfo()->left;
    m_mapmarg.coord.lat_start = m_mapmarg.coord.lat_start > mapinfo->getMapInfo()->down ?  m_mapmarg.coord.lat_start : mapinfo->getMapInfo()->down;
    m_mapmarg.coord.long_stop = m_mapmarg.coord.long_stop < mapinfo->getMapInfo()->right ?  m_mapmarg.coord.long_stop : mapinfo->getMapInfo()->right;
    m_mapmarg.coord.lat_stop = m_mapmarg.coord.lat_stop < mapinfo->getMapInfo()->up ?  m_mapmarg.coord.lat_stop : mapinfo->getMapInfo()->up;

#ifdef _DEBUG_MapFBODataServer_
    qDebug()<<"\n---------- MapFBODataServer.cpp::loadMapSlice ------------- \n";
    qDebug()<<"long_start="<< m_mapmarg.coord.long_start<<", lat_start="<< m_mapmarg.coord.lat_start<<", long_stop="<< m_mapmarg.coord.long_stop<<", lat_stop="<< m_mapmarg.coord.lat_stop<<"\n";
#endif

    m_drawproperties.res_xy =  ((float)DifMapCoord(m_mapmarg.coord.lat_stop, m_mapmarg.coord.lat_start) / (float)DifMapCoord(m_mapmarg.coord.long_stop, m_mapmarg.coord.long_start) );

    calcDrawBBoxes(m_mapmarg.bbox, m_mapmarg.coord);
    calcDrawResolution();
}

void MapFBODataServer::moveSlice (E_MoveDirection direction)
{
    int step = m_mapp.zoom;
//      float slide_x = step / m_drawproperties.res_x + 1;
//      float slide_y = step / m_drawproperties.res_y + 1;

    DT_stDrawRegion draw_region;
    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

    DT_stMapMarginsCoord draw_coord;
    draw_coord.long_start = m_mapmarg.coord.long_start;
    draw_coord.long_stop = m_mapmarg.coord.long_stop;
    draw_coord.lat_start = m_mapmarg.coord.lat_start;
    draw_coord.lat_stop = m_mapmarg.coord.lat_stop;

    QImage m_pimage_buf;
    switch (direction) {
        case E_Left:
            if (DifMapCoord(m_mapmarg.coord.long_start, step) <= mapinfo->getMapInfo()->left)
                return;
//              m_pimage_buf = m_pimage->copy(0, 0, m_drawproperties.width - slide_x, m_drawproperties.height);
//              m_ppainter->end();
//              *m_pimage = m_pimage_buf.copy(- slide_x , 0, m_drawproperties.width, m_drawproperties.height);
            m_mapmarg.coord.long_start = DifMapCoord(m_mapmarg.coord.long_start, step);
            m_mapmarg.coord.long_stop = DifMapCoord(m_mapmarg.coord.long_stop, step);
//              draw_region.x2 = slide_x;
//              draw_coord.long_stop = m_mapmarg.coord.long_start + step;
            break;
        case E_Right:
            if (AddMapCoord(m_mapmarg.coord.long_stop, step) >= mapinfo->getMapInfo()->right)
                return;
//              m_pimage_buf = m_pimage->copy(slide_x, 0, m_drawproperties.width - slide_x, m_drawproperties.height);
//              m_ppainter->end();
//              *m_pimage = m_pimage_buf.copy(0 , 0, m_drawproperties.width, m_drawproperties.height);
            m_mapmarg.coord.long_start = AddMapCoord(m_mapmarg.coord.long_start, step);
            m_mapmarg.coord.long_stop = AddMapCoord(m_mapmarg.coord.long_stop, step);
//              draw_region.x1 = m_drawproperties.width - slide_x;
//              draw_coord.long_start = m_mapmarg.coord.long_stop - step;
            break;
        case E_Down:
            if      (DifMapCoord(m_mapmarg.coord.lat_start, step) <= mapinfo->getMapInfo()->down)
                return;
//              m_pimage_buf = m_pimage->copy(0, slide_y, m_drawproperties.width, m_drawproperties.height - slide_y);
//              m_ppainter->end();
//              *m_pimage = m_pimage_buf.copy(0 , 0, m_drawproperties.width, m_drawproperties.height);
            m_mapmarg.coord.lat_start = DifMapCoord(m_mapmarg.coord.lat_start, step);
            m_mapmarg.coord.lat_stop = DifMapCoord(m_mapmarg.coord.lat_stop, step);
//              draw_region.y1 = m_drawproperties.height - slide_y;
//              draw_coord.lat_stop = m_mapmarg.coord.lat_start + step;
            break;
        case E_Up:
            if      (AddMapCoord(m_mapmarg.coord.lat_stop, step) >= mapinfo->getMapInfo()->up)
                return;
//              m_pimage_buf = m_pimage->copy(0, -slide_y, m_drawproperties.width, m_drawproperties.height);
//              m_ppainter->end();
//              *m_pimage = m_pimage_buf.copy(0 , 0, m_drawproperties.width, m_drawproperties.height);
            m_mapmarg.coord.lat_start = AddMapCoord(m_mapmarg.coord.lat_start, step);
            m_mapmarg.coord.lat_stop = AddMapCoord(m_mapmarg.coord.lat_stop, step);
//              draw_region.y2 = slide_y;
//              draw_coord.lat_start = m_mapmarg.coord.lat_stop - step;
            break;
        default:
            break;
    }
//      m_ppainter->begin(m_pimage);
//      m_ppainter->setRenderHint(QPainter::Antialiasing);
    calcDrawBBoxes(m_mapmarg.bbox, draw_coord);
    renderMapData(draw_region, draw_coord);

#ifdef _DEBUG_MapFBODataServer_
    qDebug()<<"\n---------- MapFBODataServer.cpp::moveSlice ------------- \n";
    qDebug()<<"long_start="<< m_mapmarg.coord.long_start<<", lat_start="<< m_mapmarg.coord.lat_start<<", long_stop="<< m_mapmarg.coord.long_stop<<", lat_stop="<< m_mapmarg.coord.lat_stop<<"\n";
#endif
}

void MapFBODataServer::zoomSlice (E_ZoomType zoom)
{
    int step_x = (10 * m_mapp.zoom);
    int step_y = m_drawproperties.res_xy * step_x;
    DT_stDrawRegion draw_region;

    switch (zoom) {
        case E_ZoomIn:
            // minimum zoom
            if ((step_x <= 200) || (step_y <= 200))
                return;
            m_mapmarg.coord.long_start = AddMapCoord(m_mapmarg.coord.long_start, step_x);
            m_mapmarg.coord.long_stop = DifMapCoord(m_mapmarg.coord.long_stop, step_x);
            m_mapmarg.coord.lat_start = AddMapCoord(m_mapmarg.coord.lat_start, step_y);
            m_mapmarg.coord.lat_stop = DifMapCoord(m_mapmarg.coord.lat_stop, step_y);

            break;
        case E_ZoomOut:
            if ((DifMapCoord(m_mapmarg.coord.long_start, step_x) >= mapinfo->getMapInfo()->left) &&
                (AddMapCoord(m_mapmarg.coord.long_stop, step_x) <= mapinfo->getMapInfo()->right) &&
                (DifMapCoord(m_mapmarg.coord.lat_start, step_y) >= mapinfo->getMapInfo()->down) &&
                (AddMapCoord(m_mapmarg.coord.lat_stop, step_y) <= mapinfo->getMapInfo()->up))
            {
                m_mapmarg.coord.long_start = DifMapCoord(m_mapmarg.coord.long_start, step_x);
                m_mapmarg.coord.long_stop = AddMapCoord(m_mapmarg.coord.long_stop, step_x);
                m_mapmarg.coord.lat_start = DifMapCoord(m_mapmarg.coord.lat_start, step_y);
                m_mapmarg.coord.lat_stop = AddMapCoord(m_mapmarg.coord.lat_stop, step_y);
            }
            break;
    }

#ifdef _DEBUG_MapFBODataServer_
    qDebug()<<"\n---------- MapFBODataServer.cpp::zoomSlice ------------- \n";
    qDebug()<<"long_start="<< m_mapmarg.coord.long_start<<", lat_start="<< m_mapmarg.coord.lat_start<<", long_stop="<< m_mapmarg.coord.long_stop<<", lat_stop="<< m_mapmarg.coord.lat_stop<<"\n";
#endif

    calcDrawBBoxes(m_mapmarg.bbox, m_mapmarg.coord);
    calcDrawResolution();
    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;
    renderMapData(draw_region, m_mapmarg.coord);
}


void MapFBODataServer::moveSliceMouse(DT_stMapMarginsCoord coord)
{
    DT_stDrawRegion draw_region;
    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;
//    printf("Zoom: %d\n",m_mapp.zoom);

    DT_stMapMarginsCoord draw_coord;
    draw_coord.long_start = m_mapmarg.coord.long_start;
    draw_coord.long_stop = m_mapmarg.coord.long_stop;
    draw_coord.lat_start = m_mapmarg.coord.lat_start;
    draw_coord.lat_stop = m_mapmarg.coord.lat_stop;

    m_mapmarg.coord.lat_start = coord.lat_start;
    m_mapmarg.coord.lat_stop = coord.lat_stop;
    m_mapmarg.coord.long_start = coord.long_start;
    m_mapmarg.coord.long_stop = coord.long_stop;

    calcDrawBBoxes(m_mapmarg.bbox,draw_coord);
    renderMapData(draw_region, draw_coord);

}

void MapFBODataServer::renderBBoxPolygons (int horiz_bbox_no, int vert_bbox_no, int zoom_level)
{
    unsigned int bboxid = horiz_bbox_no + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

    // Check the tile server status
    if (m_tileServer == NULL) {
        m_tileServer = new CTileServer();
    }
    else {
        m_tileServer->setBBox(bboxid);
        m_tileServer->setZoomLevel(zoom_level);
    }
    m_tileServer->LoadBBox();
    printf("---------------------------------------\n");
    printf("BBox: %d    zoom level: %d\n", bboxid, zoom_level);
    printf("---------------------------------------\n");

    for(DT_OSMAreaBBox* bbox_area = m_tileServer->getFirstArea(); bbox_area; bbox_area = m_tileServer->getNextArea())
    {
        int new_vert_bbox_no = vert_bbox_no;
        int new_horiz_bbox_no = horiz_bbox_no;
        unsigned int prevBBox=bbox_area->prevBBoxId, prevNodeId=bbox_area->prevBBoxNodeId;
        bool isDrawn=false;

        // check if current area is already drawn
//    	if(bbox_area->id==88597518)
//    		printf("%d %d\n", bbox_area->prevBBoxId, bboxid);
        if(bbox_area->id!=88597518)
            continue;
        printf("Check for prev\n");
//    	while(prevBBox)
//    	{
////    		printf("%d %d\n", bbox_area->id, prevBBox);
//    		printf("Found\n");
//    		prevBBox--;
//    		new_vert_bbox_no = prevBBox / mapinfo->getMapInfo()->horBBoxNo;
//    		new_horiz_bbox_no = prevBBox - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
//
//    		if(prevBBox<bboxid && new_vert_bbox_no>=m_mapmarg.bbox.vert_start && new_horiz_bbox_no>=m_mapmarg.bbox.horiz_start)
//    		{
//    			isDrawn=true;
//    			break;
//    		}
//
//    		m_tileServer->setBBox(prevBBox);
//			m_tileServer->LoadBBox();
//
//			int areaCnt = m_tileServer->countAreas(bbox_area->id, prevBBox, bbox_area->zoomLevel);
//			if(bbox_area->id==88597518)
//			printf("Area count: %d\n", areaCnt);
//
//    		while(areaCnt--)
//    		{
//    			bbox_area = m_tileServer->getArea(bbox_area->id, prevBBox, bbox_area->zoomLevel, areaCnt);
//				if (bbox_area->nextBBoxNodeId == prevNodeId)
//				{
//					printf("change area\n");
//					prevNodeId=bbox_area->prevBBoxNodeId;
//					prevBBox=bbox_area->prevBBoxId;
//					break;
//				}
//    		}
//    	}

//    	if(isDrawn)
//    		continue;

        int tmpdrawcoord_x = 0, tmpdrawcoord_y = 0;
        QPolygon polygon;

        int horiz_coordoffset = (new_horiz_bbox_no - m_mapmarg.bbox.horiz_start) * BBOXSIZE - (CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, m_mapmarg.coord.long_start) - m_mapmarg.bbox.horiz_start * BBOXSIZE);
        int vert_coordoffset = (m_mapmarg.bbox.vert_size - (new_vert_bbox_no - m_mapmarg.bbox.vert_start)) * BBOXSIZE -  ( (m_mapmarg.bbox.vert_start + m_mapmarg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, m_mapmarg.coord.lat_stop));
//        printf("Area: %d %d\n", bbox_area->id, bbox_area->referencesNo);
        for (int i = 0; i < bbox_area->referencesNo; i++)
        {
            DT_BBoxNode *node = m_tileServer->getAreaNode(bbox_area->referenceList[i], bbox_area->zoomLevel);

            tmpdrawcoord_x = (node->bbox_long + horiz_coordoffset) / m_drawproperties.res_x;
            tmpdrawcoord_y = (vert_coordoffset - node->bbox_lat) / m_drawproperties.res_y;
            m_ppainter->setPen(QPen(Qt::gray, 1, Qt::SolidLine, Qt::RoundCap));
            m_ppainter->drawEllipse(tmpdrawcoord_x-5, tmpdrawcoord_y-5, 11, 11);
            printf("Coord %d: %d %d\n", i , tmpdrawcoord_x, tmpdrawcoord_y);
            printf("bbox: %d  --  %d %d \n\n", bboxid, node->bbox_lat, node->bbox_long);

            polygon << QPoint(tmpdrawcoord_x, tmpdrawcoord_y);
        }

        m_ppainter->drawPolygon(polygon);

        m_tileServer->setBBox(bboxid);
//        m_tileServer->LoadBBox();
    }
}

void MapFBODataServer::renderBBox (int horiz_bbox_no, int vert_bbox_no, int zoom_level, int render_step, DT_stDrawRegion draw_region)
{
    unsigned int bboxid = horiz_bbox_no  + (vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo);

    int horiz_coordoffset = (horiz_bbox_no - m_mapmarg.bbox.horiz_start) * BBOXSIZE - (CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left, m_mapmarg.coord.long_start) - m_mapmarg.bbox.horiz_start * BBOXSIZE);
    int vert_coordoffset = (m_mapmarg.bbox.vert_size - (vert_bbox_no - m_mapmarg.bbox.vert_start)) * BBOXSIZE -  ( (m_mapmarg.bbox.vert_start + m_mapmarg.bbox.vert_size) * BBOXSIZE - CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down, m_mapmarg.coord.lat_stop));

    m_ppainter->setPen(QPen(Qt::darkBlue, 1, Qt::SolidLine, Qt::RoundCap));
    m_ppainter->drawText(horiz_coordoffset/m_drawproperties.res_x-5, vert_coordoffset/m_drawproperties.res_y-5, QString::number(bboxid));

    // Check the tile server status
    if (m_tileServer == NULL) {
        m_tileServer = new CTileServer();
    }
    else {
        m_tileServer->setBBox(bboxid);
        m_tileServer->setZoomLevel(zoom_level);
    }
    m_tileServer->LoadBBox();

        //draw all the nodes in the current bbox
//    if(m_mapp.zoom<400)
//    {
//    	CNodesServer *nodesServer=m_tileServer->getNodeServer();
//    	printf("%d\n", nodesServer->getNodesNo());
//
//        DT_BBoxNode *nd=nodesServer->getFirstNode(bboxid, zoom_level);
//        while(nd)
//        {
//            int x = (nd->bbox_long + horiz_coordoffset) / m_drawproperties.res_x;
//            int y = (vert_coordoffset - nd->bbox_lat) / m_drawproperties.res_y;
//
//            m_ppainter->setPen(QPen(Qt::gray, 1, Qt::SolidLine, Qt::RoundCap));
//            m_ppainter->drawEllipse(x-5, y-5, 11, 11);
//
//            nd=nodesServer->getNextNode(bboxid,  zoom_level);
//        }
//    }

    // start with first way
    DT_OSMWayBBox* bbox_way = m_tileServer->getFirstWay();
    while (bbox_way != NULL)
    {
        int draw_startx = 0, draw_starty = 0, tmpdrawcoord_x = 0, tmpdrawcoord_y = 0;
        QPainterPath path;

        for (int i = 0; i < bbox_way->referencesNo; i++)
        {
            DT_BBoxNode *node = m_tileServer->getWayNode(bbox_way->referenceList[i], bbox_way->zoomLevel);

            // Error proof : normally we don't reach
            if (node == NULL) {
                break;
            }

            tmpdrawcoord_x = (node->bbox_long + horiz_coordoffset) / m_drawproperties.res_x;
            tmpdrawcoord_y = (vert_coordoffset - node->bbox_lat) / m_drawproperties.res_y;

            //draw way id over each node
//            if(m_mapp.zoom<200)
//            {
//                m_ppainter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
//                m_ppainter->drawText(tmpdrawcoord_x, tmpdrawcoord_y-40, QString::number(bbox_way->id));
//            }

            if (i == 0) {
                draw_startx = tmpdrawcoord_x; draw_starty = tmpdrawcoord_y;
                path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
            }
            else
                path.lineTo(tmpdrawcoord_x, tmpdrawcoord_y);
        }

        // Check if the way continues in other BBox
        if (bbox_way->nextBBoxId != 0) {
            int nextBboxId = bbox_way->nextBBoxId - 1;
            m_tileServer->setBBox(nextBboxId);
            m_tileServer->LoadBBox();
            int waycnt = m_tileServer->countWays(bbox_way->id, nextBboxId, bbox_way->zoomLevel);
            while (waycnt > 0) {
                DT_OSMWayBBox* way = m_tileServer->getWay(bbox_way->id, nextBboxId, bbox_way->zoomLevel, waycnt - 1);
                if (way->prevBBoxNodeId == bbox_way->nextBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getWayNode(way->referenceList[0], bbox_way->zoomLevel);

                    int new_vert_bbox_no = nextBboxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = nextBboxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = (node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE) / m_drawproperties.res_x;
                    int coord_y = (vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat) / m_drawproperties.res_y;
                    if (((coord_x >= draw_region.x1) && (coord_x <= draw_region.x2) &&
                        (coord_y >= draw_region.y1) && (coord_y <= draw_region.y2)) ||
                        ((tmpdrawcoord_x >= draw_region.x1) && (tmpdrawcoord_x <= draw_region.x2) &&
                        (tmpdrawcoord_y >= draw_region.y1) && (tmpdrawcoord_y <= draw_region.y2)))
                    {
                        path.moveTo(tmpdrawcoord_x, tmpdrawcoord_y);
                        path.lineTo(coord_x, coord_y);
                    }
                }
                waycnt--;
            }
        }

        if (bbox_way->prevBBoxId != 0) {
            int prevBBoxId = bbox_way->prevBBoxId - 1;
            m_tileServer->setBBox(prevBBoxId);
            m_tileServer->LoadBBox();
            int waycnt = m_tileServer->countWays(bbox_way->id, prevBBoxId, bbox_way->zoomLevel);
            while (waycnt > 0) {
                DT_OSMWayBBox* way = m_tileServer->getWay(bbox_way->id, prevBBoxId, bbox_way->zoomLevel, waycnt - 1);
                if (way->nextBBoxNodeId == bbox_way->prevBBoxNodeId) {
                    DT_BBoxNode *node = m_tileServer->getWayNode(way->referenceList[way->referencesNo - 1], bbox_way->zoomLevel);

                    int new_vert_bbox_no = prevBBoxId / mapinfo->getMapInfo()->horBBoxNo;
                    int new_horiz_bbox_no = prevBBoxId - new_vert_bbox_no * mapinfo->getMapInfo()->horBBoxNo;
                    int coord_x = (node->bbox_long + horiz_coordoffset + (new_horiz_bbox_no - horiz_bbox_no) * BBOXSIZE) / m_drawproperties.res_x;
                    int coord_y = (vert_coordoffset + (vert_bbox_no - new_vert_bbox_no) * BBOXSIZE- node->bbox_lat) / m_drawproperties.res_y;
                    if (((coord_x >= draw_region.x1) && (coord_x <= draw_region.x2) &&
                        (coord_y >= draw_region.y1) && (coord_y <= draw_region.y2)) ||
                        ((draw_startx >= draw_region.x1) && (draw_startx <= draw_region.x2) &&
                        (draw_starty >= draw_region.y1) && (draw_starty <= draw_region.y2)))
                    {
                        path.moveTo(draw_startx, draw_starty);
                        path.lineTo(coord_x, coord_y);
                    }
                }
                waycnt--;
            }
        }

        m_ppainter->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));
        m_ppainter->setPen(DrawStyle::getMapDrawStyle()->setRoadPenStyle(bbox_way->type, m_mapp.zoom, render_step));
        m_ppainter->drawPath(path);

        //*************************************************************************************************************/
        //! test purpose only
//        fbo_painter->setPen(DrawStyle::getMapDrawStyle()->setRoadPenStyle(path, bbox_way->type, m_mapp.zoom, render_step));
//        fbo_painter->drawPath(path);
//        fbo_painter->end();

//        if (render_fbo != texture_fbo) {
//            QRect rect(0, 0, render_fbo->width(), render_fbo->height());
//            QGLFramebufferObject::blitFramebuffer(texture_fbo, rect, render_fbo, rect);
//        }
        //*************************************************************************************************************/

        m_tileServer->setBBox(bboxid);
        bbox_way = m_tileServer->getNextWay();
    }
}

void MapFBODataServer::renderMapData (DT_stDrawRegion draw_region, DT_stMapMarginsCoord draw_coord)
{
    int ii, jj, zl, rs=1;
    DT_stMapMarginsBBox draw_margins_bbox;
    // set background
    m_ppainter->fillRect(QRect(draw_region.x1, draw_region.y1, draw_region.x2 - draw_region.x1, draw_region.y2 - draw_region.y1),
                                    DrawStyle::getMapDrawStyle()->getBackColor());

    int zoom_level = NO_ZOOMLEVELS - (m_mapp.zoom  / 2000);
    if (zoom_level <= 0) zoom_level = 1;
    if (zoom_level > 5) zoom_level = 5;

    calcDrawBBoxes(draw_margins_bbox, draw_coord);
//printf("#################################################\n%d %d\n", draw_margins_bbox.horiz_size, draw_margins_bbox.vert_size);
//   	m_ppainter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine, Qt::RoundCap));
//    for (ii = draw_margins_bbox.horiz_start; ii < (draw_margins_bbox.horiz_start + draw_margins_bbox.horiz_size) ; ii++)
//	{
//		for (jj = draw_margins_bbox.vert_start; jj < (draw_margins_bbox.vert_start + draw_margins_bbox.vert_size); jj++)
//		{
//			for (zl = zoom_level ; zl >= 0; zl--)
//	        	renderBBoxPolygons(ii, jj, zl);
//		}
//	}

    // draw each bbox
    for (rs = 0; rs <= 1 ; rs++)
    {
        for (ii = draw_margins_bbox.horiz_start; ii < (draw_margins_bbox.horiz_start + draw_margins_bbox.horiz_size) ; ii++)
        {
            for (jj = draw_margins_bbox.vert_start; jj < (draw_margins_bbox.vert_start + draw_margins_bbox.vert_size); jj++)
            {
                for (zl = zoom_level ; zl >= 0; zl--)
                {
                    renderBBox(ii, jj, zl, rs, draw_region);
                }
            }
        }
        // perform just one step for large zoom levels
        if (m_mapp.zoom > 1000)
            break;
    }


    //draw the bbox vertical and horizontal delimitation lines
    m_ppainter->setPen(QPen(Qt::red, 1.5, Qt::DashDotLine, Qt::RoundCap));
    for (ii = draw_margins_bbox.horiz_start; ii < (draw_margins_bbox.horiz_start + draw_margins_bbox.horiz_size) ; ii++)
        m_ppainter->drawLine((ii*BBOXSIZE-CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left,m_mapmarg.coord.long_start))/m_drawproperties.res_x, 0, (ii*BBOXSIZE-CovertDecDeg2MapUnits(mapinfo->getMapInfo()->left,m_mapmarg.coord.long_start))/m_drawproperties.res_x, m_drawproperties.height);

    for (jj = draw_margins_bbox.vert_start; jj < (draw_margins_bbox.vert_start + draw_margins_bbox.vert_size); jj++)
        m_ppainter->drawLine(0, m_drawproperties.height-(jj*BBOXSIZE-CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down,m_mapmarg.coord.lat_start))/m_drawproperties.res_y, m_drawproperties.width, m_drawproperties.height-(jj*BBOXSIZE-CovertDecDeg2MapUnits(mapinfo->getMapInfo()->down,m_mapmarg.coord.lat_start))/m_drawproperties.res_y);

    emit dataReady();
}

void MapFBODataServer::run ()
{
    DT_stDrawRegion draw_region;

    draw_region.x1 = 0; draw_region.x2 = m_drawproperties.width;
    draw_region.y1 = 0; draw_region.y2 = m_drawproperties.height;

    renderMapData(draw_region, m_mapmarg.coord);
    exec();
}
