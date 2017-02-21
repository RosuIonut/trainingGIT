/*
 * PosDotDraw.h
 *
 *  Created on: Aug 14, 2012
 *      Author: eu
 */

#ifndef POSDOTDRAW_H_
#define POSDOTDRAW_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QTimer>

#include "DataStructures.h"
#include "DataTypes.h"

class PosDotDraw : public QWidget
{
	Q_OBJECT
public:
	explicit PosDotDraw(QWidget *parent, int width, int height);
    ~PosDotDraw();

public slots:
	void onPositionChanged(DT_MapCoordinate latitude, DT_MapCoordinate longitude);
	void onPropertiesChanged(DT_stDrawProperties*&);
	void onAnimateTimer();
	void onImageSlide(int x, int y);

protected:
    void paintEvent(QPaintEvent *event);

    bool m_bState;
    QTimer* m_pAnimateTimer;

    int m_iPosX;
    int m_iPosY;

    int m_iImgX;
    int m_iImgY;

    DT_MapCoordinate m_viewLatitude;
    DT_MapCoordinate m_viewLongitude;

    DT_stDrawProperties m_viewProperties;
};

#endif /* POSDOTDRAW_H_ */
