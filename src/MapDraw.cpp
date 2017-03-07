#include "MapDraw.h"
#include "TileServer.h"
//merge?
//#define _DEBUG_MAP_DRAW_

MapDraw::MapDraw(QWidget* parent, int width, int height) :
	QWidget(parent),
	m_tileServer(new CTileServer),
	m_drawproperties(new DT_stDrawProperties),
	m_image(new QImage(width * 3, height * 3, QImage::Format_RGB16)),
	m_mapImageDataServer(new MapImageDataServer(this, m_tileServer, m_drawproperties, m_image))
{
#ifdef _MAP_DRAW_DEBUG_
	qDebug()<<"this->width()="<< this->width()<<" ; this->height()="<<this->height());
#endif

	this->resize(width, height);

	double long_start, long_stop, lat_start, lat_stop;
	long_start = 23.59;	long_stop = 23.63354; lat_start = 46.77; lat_stop = 46.78299; //cluj
//	long_start = 24.5654, long_stop = 24.6058, lat_start = 46.5368, lat_stop = 46.554; //mures
//	long_start = 25.50;	long_stop = 26.5; lat_start = 43.70; lat_stop = 44.70; //bucuresti
//	long_start = 26.2229;	long_stop = 26.27496; lat_stop = 47.65849; lat_start = 47.63065; //suceava
//	long_start = 24.76215;	long_stop = 24.76835; lat_start = 46.53265; lat_stop = 46.53565; //nowhere

	m_drawproperties->bufferWidth=width * 3;
	m_drawproperties->bufferHeight=height * 3;

	m_drawproperties->startX=width;
	m_drawproperties->startY=height;
	m_drawproperties->width=width;
	m_drawproperties->height=height;

	m_drawproperties->res_x = m_drawproperties->res_y = 0;
	m_drawproperties->zoom=300;
	m_drawproperties->res_xy = (double)height/width * 0.65;

	imageStartPosition=QPoint(0, 0);

	setLocation(long_start, lat_start);
}

MapDraw::~MapDraw()
{
	delete m_mapImageDataServer;
	delete m_image;
	delete m_drawproperties;
	delete m_tileServer;
}

void MapDraw::onImageSlide(int x, int y)
{
	imageStartPosition=QPoint(x, y);
	update();
}


void MapDraw::moveMap(int x, int y)
{
	m_mapImageDataServer->stop();
	m_mapImageDataServer->wait();

	DT_MapCoordinate x1=m_drawproperties->bufferMarg.long_start, x2=m_drawproperties->bufferMarg.long_stop, y1=m_drawproperties->bufferMarg.lat_start, y2=m_drawproperties->bufferMarg.lat_stop;

	if(x<0)
	{
		m_drawproperties->bufferMarg.long_start=AddMapUnits(m_drawproperties->bufferMarg.long_start, ((-x)*m_drawproperties->res_x));
		m_drawproperties->bufferMarg.long_stop=AddMapUnits(m_drawproperties->bufferMarg.long_stop, ((-x)*m_drawproperties->res_x));
		m_drawproperties->visibleMarg.long_start=AddMapUnits(m_drawproperties->visibleMarg.long_start, ((-x)*m_drawproperties->res_x));
		m_drawproperties->visibleMarg.long_stop=AddMapUnits(m_drawproperties->visibleMarg.long_stop, ((-x)*m_drawproperties->res_x));

		if(y<0)
		{
			m_drawproperties->bufferMarg.lat_start=DifMapUnits(m_drawproperties->bufferMarg.lat_start, ((-y)*m_drawproperties->res_y));
			m_drawproperties->bufferMarg.lat_stop=DifMapUnits(m_drawproperties->bufferMarg.lat_stop, ((-y)*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_start=DifMapUnits(m_drawproperties->visibleMarg.lat_start, ((-y)*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_stop=DifMapUnits(m_drawproperties->visibleMarg.lat_stop, ((-y)*m_drawproperties->res_y));

			emit mouseSlide(x, y);
			m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, x2, m_drawproperties->bufferMarg.lat_start, y1, 0, m_drawproperties->bufferHeight+y, m_drawproperties->bufferWidth+x, -y);
		}
		else if(y>0)
		{
			m_drawproperties->bufferMarg.lat_start=AddMapUnits(m_drawproperties->bufferMarg.lat_start, (y*m_drawproperties->res_y));
			m_drawproperties->bufferMarg.lat_stop=AddMapUnits(m_drawproperties->bufferMarg.lat_stop, (y*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_start=AddMapUnits(m_drawproperties->visibleMarg.lat_start, (y*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_stop=AddMapUnits(m_drawproperties->visibleMarg.lat_stop, (y*m_drawproperties->res_y));

			emit mouseSlide(x, y);
			m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, x2, y2, m_drawproperties->bufferMarg.lat_stop, 0, 0, m_drawproperties->bufferWidth+x, y);
		}

		m_mapImageDataServer->loadMapSlice(x2, m_drawproperties->bufferMarg.long_stop, m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop, m_drawproperties->bufferWidth+x, 0, -x, m_drawproperties->bufferHeight);
	}
	else
	{
		m_drawproperties->bufferMarg.long_start=DifMapUnits(m_drawproperties->bufferMarg.long_start, (x*m_drawproperties->res_x));
		m_drawproperties->bufferMarg.long_stop=DifMapUnits(m_drawproperties->bufferMarg.long_stop, (x*m_drawproperties->res_x));
		m_drawproperties->visibleMarg.long_start=DifMapUnits(m_drawproperties->visibleMarg.long_start, (x*m_drawproperties->res_x));
		m_drawproperties->visibleMarg.long_stop=DifMapUnits(m_drawproperties->visibleMarg.long_stop, (x*m_drawproperties->res_x));

		if(y<0)
		{
			m_drawproperties->bufferMarg.lat_start=DifMapUnits(m_drawproperties->bufferMarg.lat_start, ((-y)*m_drawproperties->res_y));
			m_drawproperties->bufferMarg.lat_stop=DifMapUnits(m_drawproperties->bufferMarg.lat_stop, ((-y)*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_start=DifMapUnits(m_drawproperties->visibleMarg.lat_start, ((-y)*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_stop=DifMapUnits(m_drawproperties->visibleMarg.lat_stop, ((-y)*m_drawproperties->res_y));

			emit mouseSlide(x, y);
			m_mapImageDataServer->loadMapSlice(x1, m_drawproperties->bufferMarg.long_stop, m_drawproperties->bufferMarg.lat_start, y1, x, m_drawproperties->bufferHeight+y, m_drawproperties->bufferWidth-x, -y);
		}
		else if(y>0)
		{
			m_drawproperties->bufferMarg.lat_start=AddMapUnits(m_drawproperties->bufferMarg.lat_start, (y*m_drawproperties->res_y));
			m_drawproperties->bufferMarg.lat_stop=AddMapUnits(m_drawproperties->bufferMarg.lat_stop, (y*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_start=AddMapUnits(m_drawproperties->visibleMarg.lat_start, (y*m_drawproperties->res_y));
			m_drawproperties->visibleMarg.lat_stop=AddMapUnits(m_drawproperties->visibleMarg.lat_stop, (y*m_drawproperties->res_y));

			emit mouseSlide(x, y);
			m_mapImageDataServer->loadMapSlice(x1, m_drawproperties->bufferMarg.long_stop, y2, m_drawproperties->bufferMarg.lat_stop, x, 0, m_drawproperties->bufferWidth-x, y);
		}

		if(x>0)
		{
			m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, x1 , m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop, 0, 0, x, m_drawproperties->bufferHeight);
		}
	}

	emit propChanged(m_drawproperties);
	m_mapImageDataServer->start();
}

void MapDraw::zoomIn(QPoint pos)
{
	if(m_drawproperties->zoom<150)
		return;

	unsigned int step_x = 20 * m_drawproperties->zoom;
	unsigned int step_y = m_drawproperties->res_xy * step_x;
	double scale_x, scale_y;

	int left = step_x*((double)(m_drawproperties->startX + pos.x())/m_drawproperties->bufferWidth);
	int right = step_x - left;
	int up = step_y*((double)(m_drawproperties->startY + pos.y())/m_drawproperties->bufferHeight);
	int down = step_y - up;

	m_mapImageDataServer->clear();
	m_mapImageDataServer->m_painter->save();

	m_drawproperties->bufferMarg.long_start = AddMapUnits(m_drawproperties->bufferMarg.long_start, left);
	m_drawproperties->bufferMarg.long_stop = DifMapUnits(m_drawproperties->bufferMarg.long_stop, right);
	m_drawproperties->bufferMarg.lat_start = AddMapUnits(m_drawproperties->bufferMarg.lat_start, down);
	m_drawproperties->bufferMarg.lat_stop = DifMapUnits(m_drawproperties->bufferMarg.lat_stop, up);

	step_x=(step_x)/m_drawproperties->res_x*1.2;
	step_y=(step_y)/m_drawproperties->res_y*1.2;
	left=(left)/m_drawproperties->res_x*1.2;
	up=(up)/m_drawproperties->res_y*1.2;

	scale_x=(m_drawproperties->bufferWidth+step_x)/(double)m_drawproperties->bufferWidth;
	scale_y=(m_drawproperties->bufferHeight+step_y)/(double)m_drawproperties->bufferHeight;

	m_mapImageDataServer->m_painter->scale(scale_x, scale_y);
	m_mapImageDataServer->m_painter->drawImage(0, 0, m_image->copy(left/scale_x, up/scale_y, m_drawproperties->bufferWidth, m_drawproperties->bufferHeight));
	m_mapImageDataServer->m_painter->restore();

	update();

	calcDrawProperties();

	m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, m_drawproperties->bufferMarg.long_stop, m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop, 0, 0, m_drawproperties->bufferWidth, m_drawproperties->bufferHeight);
	m_mapImageDataServer->start();
}

void MapDraw::zoomOut(QPoint pos)
{
	if(m_drawproperties->zoom>2000000)
		return;

	unsigned int step_x = 20 * m_drawproperties->zoom;
	unsigned int step_y = m_drawproperties->res_xy * step_x;
	double scale_x, scale_y;

	int left = step_x*((double)(m_drawproperties->startX + pos.x())/m_drawproperties->bufferWidth);
	int right = step_x - left;
	int up = step_y*((double)(m_drawproperties->startY + pos.y())/m_drawproperties->bufferHeight);
	int down = step_y - up;

	m_mapImageDataServer->clear();
	m_mapImageDataServer->m_painter->save();

	m_drawproperties->bufferMarg.long_start = DifMapUnits(m_drawproperties->bufferMarg.long_start, left);
	m_drawproperties->bufferMarg.long_stop = AddMapUnits(m_drawproperties->bufferMarg.long_stop, right);
	m_drawproperties->bufferMarg.lat_start = DifMapUnits(m_drawproperties->bufferMarg.lat_start, down);
	m_drawproperties->bufferMarg.lat_stop = AddMapUnits(m_drawproperties->bufferMarg.lat_stop, up);

	step_x=(step_x)/m_drawproperties->res_x*0.8;
	step_y=(step_y)/m_drawproperties->res_y*0.8;
	left=(left)/m_drawproperties->res_x*0.8;
	up=(up)/m_drawproperties->res_y*0.8;

	scale_x=(m_drawproperties->bufferWidth-step_x)/(double)m_drawproperties->bufferWidth;
	scale_y=(m_drawproperties->bufferHeight-step_y)/(double)m_drawproperties->bufferHeight;

	m_mapImageDataServer->m_painter->scale(scale_x, scale_y);
	m_mapImageDataServer->m_painter->drawImage(left/scale_x, up/scale_y, m_image->copy());
	m_mapImageDataServer->m_painter->restore();

	m_mapImageDataServer->m_painter->fillRect(left, 0, m_drawproperties->bufferWidth-step_x, up, QColor(0xeffdde));
	m_mapImageDataServer->m_painter->fillRect(left, m_drawproperties->bufferHeight-step_y+up, m_drawproperties->bufferWidth-step_x, step_y-up, QColor(0xeffdde));
	m_mapImageDataServer->m_painter->fillRect(0, 0, left, m_drawproperties->bufferHeight, QColor(0xeffdde));
	m_mapImageDataServer->m_painter->fillRect(m_drawproperties->bufferWidth-step_x+left, 0, step_x-left, m_drawproperties->bufferHeight, QColor(0xeffdde));

	update();

	calcDrawProperties();

	m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, m_drawproperties->bufferMarg.long_stop, m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop, 0, 0, m_drawproperties->bufferWidth, m_drawproperties->bufferHeight);
	m_mapImageDataServer->start();
}

void MapDraw::paintEvent (QPaintEvent *)
{
	QPainter painter(this);
	painter.drawImage(0, 0, *m_image, m_drawproperties->startX - imageStartPosition.x(), m_drawproperties->startY - imageStartPosition.y(), m_drawproperties->width, m_drawproperties->height);
}

void MapDraw::onPositionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude)
{
	setLocation(longitude, latitude);
}

bool MapDraw::event(QEvent* event)
{
	if(event->type()==QEvent::KeyPress)
	{
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);

		if(ke->key() == 16777235)
			zoomIn(QPoint(m_drawproperties->width/2, m_drawproperties->height/2));
		else if(ke->key() == 16777237)
			zoomOut(QPoint(m_drawproperties->width/2, m_drawproperties->height/2));

		return true;
	}
	else
		return QWidget::event(event);
}

void MapDraw::calcDrawProperties()
{
	// recalculate resolution
    long int start_x = CalculateMapCoordDif(m_drawproperties->bufferMarg.long_start, m_drawproperties->bufferMarg.long_stop);
    long int start_y = CalculateMapCoordDif(m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop);

    // quick formula to compute the zoom
    m_drawproperties->zoom = start_x / 100;
//    m_drawproperties->resPrecision = qMax(3, (int) (12-m_drawproperties->zoom/500));

    m_drawproperties->res_x = ((double) start_x)/m_drawproperties->bufferWidth;
    m_drawproperties->res_y = ((double) start_y)/m_drawproperties->bufferHeight;

    m_drawproperties->visibleMarg.long_start=AddMapUnits(m_drawproperties->bufferMarg.long_start, (m_drawproperties->startX*m_drawproperties->res_x));
	m_drawproperties->visibleMarg.long_stop=AddMapUnits(m_drawproperties->visibleMarg.long_start, (m_drawproperties->width*m_drawproperties->res_x));
	m_drawproperties->visibleMarg.lat_start=AddMapUnits(m_drawproperties->bufferMarg.lat_start, (m_drawproperties->startY*m_drawproperties->res_y));
	m_drawproperties->visibleMarg.lat_stop=AddMapUnits(m_drawproperties->visibleMarg.lat_start, (m_drawproperties->height*m_drawproperties->res_y));

	emit propChanged(m_drawproperties);
}

void MapDraw::initialize()
{
	// just this for now
	emit propChanged(m_drawproperties);
}

void MapDraw::setLocation(DT_MapCoordinate longitude, DT_MapCoordinate latitude)
{
	unsigned int lonDiff = m_drawproperties->zoom * 50;
	unsigned int latDiff = m_drawproperties->res_xy * lonDiff;

	DT_MapCoordinate long_start=DifMapUnits(longitude, lonDiff);
	DT_MapCoordinate long_stop=AddMapUnits(longitude, lonDiff);
	DT_MapCoordinate lat_start=DifMapUnits(latitude, latDiff);
	DT_MapCoordinate lat_stop=AddMapUnits(latitude, latDiff);

	if(m_drawproperties->res_x)
	{

		int xDiff = (CalculateMapCoordDif(long_start, m_drawproperties->bufferMarg.long_start))/m_drawproperties->res_x;
		int yDiff = (CalculateMapCoordDif(lat_start, m_drawproperties->bufferMarg.lat_start))/m_drawproperties->res_y;

		if(m_drawproperties->res_x && ABS(xDiff) < m_drawproperties->bufferWidth/2 && ABS(yDiff) < m_drawproperties->bufferHeight/2)
		{
			moveMap(xDiff, -yDiff);
			return;
		}
	}

	m_mapImageDataServer->clear();

	m_drawproperties->bufferMarg.long_start=long_start;
	m_drawproperties->bufferMarg.long_stop=long_stop;
	m_drawproperties->bufferMarg.lat_start=lat_start;
	m_drawproperties->bufferMarg.lat_stop=lat_stop;

	calcDrawProperties();

	m_mapImageDataServer->loadMapSlice(m_drawproperties->bufferMarg.long_start, m_drawproperties->bufferMarg.long_stop, m_drawproperties->bufferMarg.lat_start, m_drawproperties->bufferMarg.lat_stop, 0, 0, m_drawproperties->bufferWidth, m_drawproperties->bufferHeight);
	m_mapImageDataServer->start();
}
