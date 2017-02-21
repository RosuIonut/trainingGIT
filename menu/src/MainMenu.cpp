/*
 * MainMenu.cpp
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */
#include <QtDebug>
#include "menu/headers/MainMenu.h"

// 480 x 320


CMainMenu::CMainMenu(QWidget *parent_, int width_, int height_)
	:QWidget(parent_)
{
	parent = parent_;
	width = width_;
	height = height_;

	this->resize(width,height);
	QSize buttonSize(width/3,height/4);

	font = new QFont();
	font->setBold(true);
	font->setPixelSize(width/25);

	QString style =     "QToolButton {"
						"margin: 0 1px 0 1px;"
						"color: white;"
						"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
	                                "stop: 0 #2198c0, stop: 1 #0d5ca6);"
						"border-style: groove;"
						"border-radius: 10px;"
						"border-width: 0px;"
						"border-color: #79D5FC;"
						"}"

						"QToolButton:pressed {"
						"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
	                                      "stop: 0 #0d5ca6, stop: 1 #2198c0);"
						"}"
						"QToolButton:hover {"
						"border-width: 3px;"
						"}";

	poi = new QToolButton();
	poi->setFont(*font);
	poi->setText("POI");
	poi->setStyleSheet(style);
	poi->setFixedSize(buttonSize);

	address = new QToolButton();
	address->setFont(*font);
	address->setText("Address");
	address->setStyleSheet(style);
	address->setFixedSize(buttonSize);

	map = new QToolButton();
	map->setFont(*font);
	map->setText("Map");
	map->setStyleSheet(style);
	map->setFixedSize(buttonSize);

	settings = new QToolButton();
	settings->setFont(*font);
	settings->setText("Settings");
	settings->setStyleSheet(style);
	settings->setFixedSize(buttonSize);

	QPalette *p_ = new QPalette();
	p_->setColor(QPalette::WindowText,Qt::white);
	QPalette p = palette();

	QImage background("menu/images/background.jpg");
	QSize BackgroundSize(width,height);
	background = background.scaled(BackgroundSize,Qt::IgnoreAspectRatio,Qt::FastTransformation);
	p.setBrush(QPalette::Background, background);

	QSpacerItem *heightSpacerLeft = new QSpacerItem(0,height/6.4);
	QSpacerItem *heightSpacerRight = new QSpacerItem(0,height/6.4);
	QSpacerItem *widthSpacer = new QSpacerItem(width/5.33,0);

	leftButtonsLayout = new QVBoxLayout();
	leftButtonsLayout->addWidget(map);
	leftButtonsLayout->addSpacerItem(heightSpacerLeft);
	leftButtonsLayout->addWidget(poi);

	rightButtonsLayout = new QVBoxLayout();
	rightButtonsLayout->addWidget(address);
	rightButtonsLayout->addSpacerItem(heightSpacerRight);
	rightButtonsLayout->addWidget(settings);

	mainLayout = new QHBoxLayout();
	mainLayout->addLayout(leftButtonsLayout);
	mainLayout->addSpacerItem(widthSpacer);
	mainLayout->addLayout(rightButtonsLayout);

	connect(poi,SIGNAL(clicked()),this,SIGNAL(PoiClicked()));
	connect(address,SIGNAL(clicked()),this,SIGNAL(AddressClicked()));
	connect(map,SIGNAL(clicked()),this,SIGNAL(MapClicked()));
	connect(settings,SIGNAL(clicked()),this,SIGNAL(SettingsClicked()));

	this->setLayout(mainLayout);
	setPalette(p);

//							/*	Opacity effect for the Widget */
//
//								QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
//								opacity->setOpacity(0.90);
//								this->setGraphicsEffect(opacity);

	this->setAutoFillBackground(true);
	this->show();

}


CMainMenu::~CMainMenu()
{
}

