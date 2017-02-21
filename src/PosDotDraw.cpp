/*
 * PosDotDraw.cpp
 *
 *  Created on: Aug 14, 2012
 *      Author: eu
 */

#include <QDebug>

#include "PosDotDraw.h"

extern "C" {
#include "MapUtils.h"
}

const int DEFAULT_TIMER_INTERVAL = 1000;

const int DEFAULT_DOT_WH = 10;

PosDotDraw::PosDotDraw(QWidget *parent, int width, int height)
	: QWidget(parent)
	, m_bState(false)
	, m_pAnimateTimer(new QTimer(this))
	, m_iPosX(DEFAULT_DOT_WH)
	, m_iPosY(DEFAULT_DOT_WH)
{
	this->resize(width, height);

    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    
    m_pAnimateTimer->setInterval(DEFAULT_TIMER_INTERVAL);
    m_pAnimateTimer->setSingleShot(false);

    connect(m_pAnimateTimer,SIGNAL(timeout()),this,SLOT(onAnimateTimer()));
    m_pAnimateTimer->start();

    m_iImgX = m_iImgY = 0;
}

PosDotDraw::~PosDotDraw()
{
	delete m_pAnimateTimer;
	m_pAnimateTimer = NULL;
}

void PosDotDraw::onPositionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude)
{
	m_viewLatitude = latitude;
	m_viewLongitude = longitude;

	m_iPosX = CalculateMapCoordDif(m_viewProperties.bufferMarg.long_start, m_viewLongitude) / m_viewProperties.res_x - m_viewProperties.startX;
	m_iPosY = CalculateMapCoordDif(m_viewLatitude, m_viewProperties.bufferMarg.lat_stop) / m_viewProperties.res_y - m_viewProperties.startY;
}

void PosDotDraw::onPropertiesChanged(DT_stDrawProperties*& prop)
{
	m_viewProperties = *prop;

	m_iPosX = CalculateMapCoordDif(m_viewProperties.bufferMarg.long_start, m_viewLongitude) / m_viewProperties.res_x - m_viewProperties.startX;
	m_iPosY = CalculateMapCoordDif(m_viewLatitude, m_viewProperties.bufferMarg.lat_stop) / m_viewProperties.res_y - m_viewProperties.startY;
}

void PosDotDraw::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    if (m_bState){
    	painter.setPen(QPen(Qt::red));
    	painter.setBrush(QBrush(Qt::red));
    }
    else{
    	painter.setPen(QPen(Qt::darkRed));
    	painter.setBrush(QBrush(Qt::darkRed));
    }

    painter.drawEllipse(m_iPosX + m_iImgX - DEFAULT_DOT_WH/2, m_iPosY + m_iImgY - DEFAULT_DOT_WH/2, DEFAULT_DOT_WH, DEFAULT_DOT_WH);
}

void PosDotDraw::onAnimateTimer()
{
	m_bState = !m_bState;
	update();
}

void PosDotDraw::onImageSlide(int x, int y)
{
	m_iImgX = x;
	m_iImgY = y;

	update();
}


