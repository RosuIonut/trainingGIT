/*
 * MenuServer.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: Ionut Rosu
 */

#include "menu/headers/MenuServer.h"


CMenuServer::CMenuServer(QWidget *parent_,int width_,int height_)
	: QWidget(parent_)
{
	resize(width_, height_);
	parent = parent_;
	width = width_;
	height = height_;
	createMenuButton();

}

CMenuServer::~CMenuServer()
{

}

void
CMenuServer::createMenuButton()
{
	menuButton = new CMenuButton(parent,width,height);

	connect(menuButton,SIGNAL(MenuClicked()),this,SLOT(createMainMenu()));
	connect(menuButton,SIGNAL(MenuClicked()),this,SLOT(destroyMenuButton()));

}

void
CMenuServer::destroyMenuButton()
{
	delete menuButton;
}

void
CMenuServer::createMainMenu()
{
	mainMenu = new CMainMenu(parent,width,height);
	connect(mainMenu,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(mainMenu,SIGNAL(MapClicked()),this,SLOT(destroyMainMenu()));

	connect(mainMenu,SIGNAL(AddressClicked()),this,SLOT(createSearchAddress()));
	connect(mainMenu,SIGNAL(AddressClicked()),this,SLOT(destroyMainMenu()));

	connect(mainMenu,SIGNAL(PoiClicked()),this,SLOT(createSearchPOI()));
	connect(mainMenu,SIGNAL(PoiClicked()),this,SLOT(destroyMainMenu()));

	connect(mainMenu,SIGNAL(SettingsClicked()),this,SLOT()); //TODO add slot
	connect(mainMenu,SIGNAL(SettingsClicked()),this,SLOT(destroyMainMenu()));

}

void
CMenuServer::destroyMainMenu()
{
	delete mainMenu;
}

void
CMenuServer::createSearchAddress()
{
	searchAddress = new CSearchAddress(parent,width,height);

	connect(searchAddress,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(searchAddress,SIGNAL(MapClicked()),this,SLOT(destroySearchAddress()));

	connect(searchAddress,SIGNAL(PreviousClicked()),this,SLOT(createMainMenu()));
	connect(searchAddress,SIGNAL(PreviousClicked()),this,SLOT(destroySearchAddress()));

	connect(searchAddress,SIGNAL(ToolCountryClicked()),this,SLOT(createSearchCountry()));
	connect(searchAddress,SIGNAL(ToolCountryClicked()),this,SLOT(hideSearchAddress()));

	connect(searchAddress,SIGNAL(ToolCityClicked()),this,SLOT(createSearchCity()));
	connect(searchAddress,SIGNAL(ToolCityClicked()),this,SLOT(hideSearchAddress()));

	connect(searchAddress,SIGNAL(ToolStreetClicked()),this,SLOT(createSearchStreet()));
	connect(searchAddress,SIGNAL(ToolStreetClicked()),this,SLOT(hideSearchAddress()));

	connect(searchAddress,SIGNAL(ToolStreetNoClicked()),this,SLOT(createSearchStreetNo()));
	connect(searchAddress,SIGNAL(ToolStreetNoClicked()),this,SLOT(hideSearchAddress()));

}

void
CMenuServer::hideSearchAddress()
{
	searchAddress->hide();
}

void
CMenuServer::showSearchAddress()
{
	searchAddress->show();
}

void
CMenuServer::destroySearchAddress()
{
	delete searchAddress;
}

void
CMenuServer::createSearchCountry()
{
	searchCountry = new CSearchCountry(parent,width,height);

	connect(searchCountry,SIGNAL(NextClicked()),this,SLOT(showSearchAddress()));
	connect(searchCountry,SIGNAL(NextClicked()),this,SLOT(setCountryName()));
	connect(searchCountry,SIGNAL(NextClicked()),this,SLOT(destroySearchCountry()));

	connect(searchCountry,SIGNAL(PreviousClicked()),this,SLOT(showSearchAddress()));
	connect(searchCountry,SIGNAL(PreviousClicked()),this,SLOT(destroySearchCountry()));

	connect(searchCountry,SIGNAL(MapClicked()),this,SLOT(destroySearchAddress()));
	connect(searchCountry,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(searchCountry,SIGNAL(MapClicked()),this,SLOT(destroySearchCountry()));

}

void
CMenuServer::setCountryName()
{
	searchAddress->setCountryName(searchCountry->GetCountryName());
}

void
CMenuServer::destroySearchCountry()
{
	delete searchCountry;
}

void
CMenuServer::createSearchCity()
{

	searchCity = new CSearchCity(parent,width,height);

	connect(searchCity,SIGNAL(NextClicked()),this,SLOT(showSearchAddress()));
	connect(searchCity,SIGNAL(NextClicked()),this,SLOT(setCityName()));
	connect(searchCity,SIGNAL(NextClicked()),this,SLOT(destroySearchCity()));

	connect(searchCity,SIGNAL(PreviousClicked()),this,SLOT(showSearchAddress()));
	connect(searchCity,SIGNAL(PreviousClicked()),this,SLOT(destroySearchCity()));

	connect(searchCity,SIGNAL(MapClicked()),this,SLOT(destroySearchAddress()));
	connect(searchCity,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(searchCity,SIGNAL(MapClicked()),this,SLOT(destroySearchCity()));

}

void
CMenuServer::setCityName()
{
	searchAddress->setCityName(searchCity->GetCityName());
}

void
CMenuServer::destroySearchCity()
{
	delete searchCity;
}

void
CMenuServer::createSearchStreet()
{

	searchStreet = new CSearchStreet(parent,width,height);

	connect(searchStreet,SIGNAL(NextClicked()),this,SLOT(showSearchAddress()));
	connect(searchStreet,SIGNAL(NextClicked()),this,SLOT(setStreetName()));
	connect(searchStreet,SIGNAL(NextClicked()),this,SLOT(destroySearchStreet()));

	connect(searchStreet,SIGNAL(PreviousClicked()),this,SLOT(showSearchAddress()));
	connect(searchStreet,SIGNAL(PreviousClicked()),this,SLOT(destroySearchStreet()));

	connect(searchStreet,SIGNAL(MapClicked()),this,SLOT(destroySearchAddress()));
	connect(searchStreet,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(searchStreet,SIGNAL(MapClicked()),this,SLOT(destroySearchStreet()));

}

void
CMenuServer::setStreetName()
{
	searchAddress->setStreetName(searchStreet->GetStreetName());
}

void
CMenuServer::destroySearchStreet()
{
	delete searchStreet;
}

void
CMenuServer::createSearchStreetNo()
{

	searchStreetNo = new CSearchStreetNo(parent,width,height);

	connect(searchStreetNo,SIGNAL(NextClicked()),this,SLOT(showSearchAddress()));
	connect(searchStreetNo,SIGNAL(NextClicked()),this,SLOT(setStreetNo()));
	connect(searchStreetNo,SIGNAL(NextClicked()),this,SLOT(destroySearchStreetNo()));

	connect(searchStreetNo,SIGNAL(PreviousClicked()),this,SLOT(showSearchAddress()));
	connect(searchStreetNo,SIGNAL(PreviousClicked()),this,SLOT(destroySearchStreetNo()));

	connect(searchStreetNo,SIGNAL(MapClicked()),this,SLOT(destroySearchAddress()));
	connect(searchStreetNo,SIGNAL(MapClicked()),this,SLOT(createMenuButton()));
	connect(searchStreetNo,SIGNAL(MapClicked()),this,SLOT(destroySearchStreetNo()));

}

void
CMenuServer::setStreetNo()
{
	searchAddress->setStreetNo(searchStreetNo->GetStreetNo());
}

void
CMenuServer::destroySearchStreetNo()
{
	delete searchStreetNo;
}

void
CMenuServer::createSearchPOI()
{
	searchPOI = new CSearchPOI(parent,width,height);

}

void
CMenuServer::destroySearchPOI()
{
	delete searchPOI;
}

void
CMenuServer::showSearchPOI()
{
	searchPOI->show();
}

void
CMenuServer::hideSearchPOI()
{
	searchPOI->hide();
}









