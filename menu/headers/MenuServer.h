/*
 * MenuServer.h
 *
 *  Created on: Oct 18, 2012
 *      Author: Ionut Rosu
 */

#ifndef MENUSERVER_H_
#define MENUSERVER_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <menu/headers/MenuButton.h>
#include <menu/headers/MainMenu.h>
#include <menu/headers/SearchAddress.h>
#include <menu/headers/SearchCountry.h>
#include <menu/headers/SearchCity.h>
#include <menu/headers/SearchStreet.h>
#include <menu/headers/SearchStreetNo.h>
#include <menu/headers/SearchPOI.h>

class CMainMenu;
class CMenuButton;
class CSearchAddress;
class CSearchCountry;
class CSearchCity;
class CSearchStreet;
class CSearchStreetNo;
class CSearchPOI;

class CMenuServer : public QWidget

{
	Q_OBJECT

public:
	explicit CMenuServer(QWidget *parent, int width, int height);
	~CMenuServer();


private slots:

void createMenuButton();
void destroyMenuButton();

void createMainMenu();
void destroyMainMenu();

void createSearchAddress();
void hideSearchAddress();
void showSearchAddress();
void destroySearchAddress();

void createSearchCountry();
void setCountryName();
void destroySearchCountry();

void createSearchCity();
void setCityName();
void destroySearchCity();

void createSearchStreet();
void setStreetName();
void destroySearchStreet();

void createSearchStreetNo();
void setStreetNo();
void destroySearchStreetNo();

void createSearchPOI();
void destroySearchPOI();
void hideSearchPOI();
void showSearchPOI();

private:

	QString countryName;
	int width,height;
	QWidget* parent;
	CMenuButton *menuButton;
	CMainMenu *mainMenu;
	CSearchAddress *searchAddress;
	CSearchCountry *searchCountry;
	CSearchCity *searchCity;
	CSearchStreet *searchStreet;
	CSearchStreetNo *searchStreetNo;
	CSearchPOI *searchPOI;


};


#endif /* MENUSERVER_H_ */
