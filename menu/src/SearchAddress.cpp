/*
 * SearchAddress.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: irosu
 */

#include "menu/headers/SearchAddress.h"

CSearchAddress::CSearchAddress(QWidget *parent_, int width_, int height_)

	:QWidget(parent_)
{
	parent = parent_;
	width = width_;
	height = height_;
	this->resize(width,height);

	CountrySet = false; CitySet = false; StreetSet = false; StreetNoSet = false;

	QSize buttonSize(width/13,width/13);
	QString ButtonStyle = 	"QPushButton {"
							"border-style: groove;"
							"border-radius: 10px;"
							"border-width: 0px;"
							"border-color: #0559F5;"
							"}"
							"QPushButton:pressed {"
							"background-color: #0A4ECC;"
							"}"
							"QPushButton:hover {"
							"border-width: 2px;"
							"}";

	QPixmap previousImg("menu/images/previous.png");

	PushPrevious = new QPushButton();
	PushPrevious->setIcon(previousImg);
	PushPrevious->setIconSize(buttonSize);
	PushPrevious->setFixedSize(buttonSize);
	PushPrevious->setFocusPolicy(Qt::NoFocus);
	PushPrevious->setFlat(true);
	PushPrevious->setStyleSheet(ButtonStyle);

	QPixmap mapImg("menu/images/map.png");

	PushMap  = new QPushButton();
	PushMap->setFixedSize(buttonSize);
	PushMap->setIcon(mapImg);
	PushMap->setIconSize(buttonSize);
	PushMap->setFocusPolicy(Qt::NoFocus);
	PushMap->setFlat(true);
	PushMap->setStyleSheet(ButtonStyle);

	QPixmap findImg("menu/images/next.png");

	PushFind = new QPushButton();
	PushFind->setIcon(findImg);
	PushFind->setFixedSize(buttonSize);
	PushFind->setIconSize(buttonSize);
	PushFind->setFocusPolicy(Qt::NoFocus);
	PushFind->setFlat(true);
	PushFind->setDisabled(true);
	PushFind->setStyleSheet(ButtonStyle);

	ToolCountry = new QToolButton();
	ToolCity = new QToolButton();
	ToolStreet = new QToolButton();
	ToolStreetNo = new QToolButton();

	QLabel *LabelCountry = new QLabel("&Country");
	QLabel *LabelCity = new QLabel("Cit&y");
	QLabel *LabelStreet = new QLabel("&Street");
	QLabel *LabelStreetNo = new QLabel("Street &No.");

	LabelCountry->setBuddy(ToolCountry);
	LabelCity->setBuddy(ToolCity);
	LabelStreet->setBuddy(ToolStreet);
	LabelStreetNo->setBuddy(ToolStreetNo);

	QPalette *p_ = new QPalette();
	p_->setColor(QPalette::WindowText,Qt::white);

	font = new QFont();
	font->setBold(true);
	font->setPixelSize(width/30);

	QFontMetrics fontMetrics(*font);
	QString labelText("Street No.");
	int labelLength = fontMetrics.width(labelText,-1);

	QString labelStyle("QLabel { background-color : transparent; }");

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
	effect->setColor(Qt::black);
	effect->setXOffset(2);
	effect->setYOffset(2);

	QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
	effect1->setColor(Qt::black);
	effect1->setXOffset(2);
	effect1->setYOffset(2);

	QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();
	effect2->setColor(Qt::black);
	effect2->setXOffset(2);
	effect2->setYOffset(2);

	QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect();
	effect3->setColor(Qt::black);
	effect3->setXOffset(2);
	effect3->setYOffset(2);

	LabelCountry->setFont(*font);
	LabelCountry->setGraphicsEffect(effect);
	LabelCountry->setStyleSheet(labelStyle);
	LabelCountry->setPalette(*p_);

	LabelCity->setFont(*font);
	LabelCity->setGraphicsEffect(effect1);
	LabelCity->setStyleSheet(labelStyle);
	LabelCity->setPalette(*p_);

	LabelStreet->setFont(*font);
	LabelStreet->setGraphicsEffect(effect2);
	LabelStreet->setStyleSheet(labelStyle);
	LabelStreet->setPalette(*p_);

	LabelStreetNo->setFont(*font);
	LabelStreetNo->setGraphicsEffect(effect3);
	LabelStreetNo->setStyleSheet(labelStyle);
	LabelStreetNo->setPalette(*p_);

	QSize toolButtonSize(width/3,height/6);

	QString ToolStyle = "QToolButton {"
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

	ToolCountry->setFixedSize(toolButtonSize);
	ToolCountry->setText("Not set");
	ToolCountry->setStyleSheet(ToolStyle);
	ToolCountry->setFont(*font);

	ToolCity->setFixedSize(toolButtonSize);
	ToolCity->setText("Not set");
	ToolCity->setStyleSheet(ToolStyle);
	ToolCity->setFont(*font);

	ToolStreet->setFixedSize(toolButtonSize);
	ToolStreet->setText("Not set");
	ToolStreet->setStyleSheet(ToolStyle);
	ToolStreet->setFont(*font);

	ToolStreetNo->setFixedSize(toolButtonSize);
	ToolStreetNo->setText("Not set");
	ToolStreetNo->setStyleSheet(ToolStyle);
	ToolStreetNo->setFont(*font);

	toolButtonsLayout = new QVBoxLayout();
	toolButtonsLayout->addWidget(ToolCountry);
	toolButtonsLayout->addWidget(ToolCity);
	toolButtonsLayout->addWidget(ToolStreet);
	toolButtonsLayout->addWidget(ToolStreetNo);

	labelLayout = new QVBoxLayout();
	labelLayout->addWidget(LabelCountry);
	labelLayout->addWidget(LabelCity);
	labelLayout->addWidget(LabelStreet);
	labelLayout->addWidget(LabelStreetNo);

	QSpacerItem *middleSpacerLeft = new QSpacerItem((width - toolButtonSize.width() - labelLength - (buttonSize.width() * 2 ))/2-15,0);
	QSpacerItem *middleSpacerRight = new QSpacerItem((width - toolButtonSize.width() - labelLength - (buttonSize.width() * 2 ))/2-15,0);

	middleLayout = new QHBoxLayout();
	middleLayout->addSpacerItem(middleSpacerLeft);
	middleLayout->addLayout(labelLayout);
	middleLayout->addLayout(toolButtonsLayout);
	middleLayout->addSpacerItem(middleSpacerRight);

	QSpacerItem *pushPreviousSpacer = new QSpacerItem(0,height-buttonSize.height());

	pushPreviousLayout = new QVBoxLayout();
	pushPreviousLayout->addSpacerItem(pushPreviousSpacer);
	pushPreviousLayout->addWidget(PushPrevious);

	QSpacerItem *pushFindSpacer = new QSpacerItem(0,height-(buttonSize.height()*2));

	pushFindLayout = new QVBoxLayout();
	pushFindLayout->addWidget(PushMap);
	pushFindLayout->addSpacerItem(pushFindSpacer);
	pushFindLayout->addWidget(PushFind);

	bottomLayout = new QHBoxLayout();
	bottomLayout->addLayout(pushPreviousLayout);
	bottomLayout->addLayout(middleLayout);
	bottomLayout->addLayout(pushFindLayout);

	mainLayout = new QVBoxLayout();
	mainLayout->addLayout(bottomLayout);

	QPalette p = palette();
	QImage background("menu/images/background.jpg");
	QSize BackgroundSize(width,height);
	background = background.scaled(BackgroundSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	p.setBrush(QPalette::Background, background);

	connect(PushMap,SIGNAL(clicked()),this,SIGNAL(MapClicked()));
	connect(PushPrevious,SIGNAL(clicked()),this,SIGNAL(PreviousClicked()));
	connect(ToolCountry,SIGNAL(clicked()),this,SIGNAL(ToolCountryClicked()));
	connect(ToolCity,SIGNAL(clicked()),this,SIGNAL(ToolCityClicked()));
	connect(ToolStreet,SIGNAL(clicked()),this,SIGNAL(ToolStreetClicked()));
	connect(ToolStreetNo,SIGNAL(clicked()),this,SIGNAL(ToolStreetNoClicked()));

	this->setLayout(mainLayout);
	this->setPalette(p);
	this->setAutoFillBackground(true);
	this->show();

}

CSearchAddress::~CSearchAddress()
{

}

void
CSearchAddress::setCountryName(QString country_name)
{
	ToolCountry->setText(country_name);

	if (!country_name.isEmpty())
		CountrySet = true;
	else
		CountrySet = false;
	if (StreetSet && CitySet && CountrySet && StreetNoSet)
		PushFind->setDisabled(false);
	else
		PushFind->setDisabled(true);
}

void
CSearchAddress::setCityName(QString city_name)
{
	ToolCity->setText(city_name);

	if (!city_name.isEmpty())
		CitySet = true;
	else
		CitySet = false;
	if (StreetSet && CitySet && CountrySet && StreetNoSet)
		PushFind->setDisabled(false);
	else
		PushFind->setDisabled(true);
}

void
CSearchAddress::setStreetName(QString street_name)
{
	ToolStreet->setText(street_name);

	if (!street_name.isEmpty())
		StreetSet = true;
	else
		StreetSet = false;
	if (StreetSet && CitySet && CountrySet && StreetNoSet)
		PushFind->setDisabled(false);
	else
		PushFind->setDisabled(true);
}

void
CSearchAddress::setStreetNo(QString street_no)
{
	ToolStreetNo->setText(street_no);

	if (!street_no.isEmpty())
		StreetNoSet = true;
	else
		StreetNoSet = false;
	if (StreetSet && CitySet && CountrySet && StreetNoSet)
		PushFind->setDisabled(false);
	else
		PushFind->setDisabled(true);
}
