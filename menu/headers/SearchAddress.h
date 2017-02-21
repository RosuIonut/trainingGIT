/*
 * SearchAddress.h
 *
 *  Created on: Oct 9, 2012
 *      Author: irosu
 */

#ifndef SEARCHADDRESS_H_
#define SEARCHADDRESS_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <menu/headers/SearchCountry.h>

class CSearchAddress : public QWidget
{
	Q_OBJECT

public:
	explicit CSearchAddress(QWidget *parent, int width, int height);
	~CSearchAddress();
	void setCountryName(QString country_name);
	void setCityName(QString city_name);
	void setStreetName(QString street_name);
	void setStreetNo(QString street_no);

signals:

	void MapClicked();
	void PreviousClicked();
	void ToolCountryClicked();
	void ToolCityClicked();
	void ToolStreetClicked();
	void ToolStreetNoClicked();

private:

	QWidget* parent;
	int width,height;

	QToolButton *ToolCountry,*ToolCity,*ToolStreet, *ToolStreetNo;
	QPushButton *PushPrevious,*PushMap,*PushFind;
	QVBoxLayout *toolButtonsLayout, *labelLayout,*pushPreviousLayout,*pushFindLayout, *mainLayout;

	QHBoxLayout *middleLayout,*bottomLayout;

	QFont *font;
	bool CountrySet,CitySet,StreetSet,StreetNoSet;


};

#endif /* SEARCHADDRESS_H_ */
