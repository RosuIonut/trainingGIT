/*
 * SearchPOI.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Rosu Ionut
 */
#include <QtDebug>
#include "menu/headers/SearchPOI.h"

CSearchPOI::CSearchPOI(QWidget *parent_, int width_, int height_)
	:QWidget(parent_)
{
	parent = parent_;
	width = width_;
	height = height_;

	this->resize(width,height);
	QSize buttonSize(width/3,height/4);

	font = new QFont();
	font->setBold(true);
	font->setPixelSize(width/35);

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

	toolAddress = new QToolButton();
	toolAddress->setFont(*font);
	toolAddress->setText("At address");
	toolAddress->setStyleSheet(style);
	toolAddress->setFixedSize(buttonSize);

	toolType = new QToolButton();
	toolType->setFont(*font);
	toolType->setText("By type");
	toolType->setStyleSheet(style);
	toolType->setFixedSize(buttonSize);

	toolCurrentLocation = new QToolButton();
	toolCurrentLocation->setFont(*font);
	toolCurrentLocation->setText("At current location");
	toolCurrentLocation->setStyleSheet(style);
	toolCurrentLocation->setFixedSize(buttonSize);

	toolByName = new QToolButton();
	toolByName->setFont(*font);
	toolByName->setText("By name");
	toolByName->setStyleSheet(style);
	toolByName->setFixedSize(buttonSize);

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
	leftButtonsLayout->addWidget(toolAddress);
	leftButtonsLayout->addSpacerItem(heightSpacerLeft);
	leftButtonsLayout->addWidget(toolType);

	rightButtonsLayout = new QVBoxLayout();
	rightButtonsLayout->addWidget(toolCurrentLocation);
	rightButtonsLayout->addSpacerItem(heightSpacerRight);
	rightButtonsLayout->addWidget(toolByName);

	mainLayout = new QHBoxLayout();
	mainLayout->addLayout(leftButtonsLayout);
	mainLayout->addSpacerItem(widthSpacer);
	mainLayout->addLayout(rightButtonsLayout);

	connect(toolAddress,SIGNAL(clicked()),this,SIGNAL(addressClicked()));
	connect(toolType,SIGNAL(clicked()),this,SIGNAL(typeClicked()));
	connect(toolCurrentLocation,SIGNAL(clicked()),this,SIGNAL(currentLocationClicked()));
	connect(toolByName,SIGNAL(clicked()),this,SIGNAL(byNameClicked()));

	setLayout(mainLayout);
	setPalette(p);
	setAutoFillBackground(true);
	show();

}

CSearchPOI::~CSearchPOI()
{

}



