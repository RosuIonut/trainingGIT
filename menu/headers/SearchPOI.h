/*
 * SearchPOI.h
 *
 *  Created on: Nov 19, 2012
 *      Author: root
 */

#ifndef SEARCHPOI_H_
#define SEARCHPOI_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>

class CSearchPOI: public QWidget{

	Q_OBJECT

public:
	 CSearchPOI(QWidget *parent,int width,int height);
	 ~CSearchPOI();

signals:
	void byNameClicked();
	void currentLocationClicked();
	void addressClicked();
	void typeClicked();

private:
	int width,height;
	QWidget *parent;

	QToolButton *toolByName,*toolCurrentLocation,*toolAddress,*toolType;
	QFont *font;
	QVBoxLayout *leftButtonsLayout, *rightButtonsLayout;
	QHBoxLayout *mainLayout;

};

#endif /* SEARCHPOI_H_ */
