/*
 * SearchCountry.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: root
 */

#include "menu/headers/SearchCountry.h"


CSearchCountry::CSearchCountry(QWidget *parent_, int width_, int height_)

	:QWidget(parent_)
{
	parent = parent_;
	width = width_;
	height = height_;
	this->resize(width,height);

	keyboard = new COnScreenKeyboard(parent,width,height);

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

	next = new QPushButton();
	previous = new QPushButton();
	map  = new QPushButton();
	QSize buttonSize(width/13,width/13);

	QPixmap nextImg("menu/images/next.png");
	next->setIcon(nextImg);
	next->setFixedSize(buttonSize);
	next->setIconSize(buttonSize);
	next->setFocusPolicy(Qt::NoFocus);
	next->setFlat(true);
	next->setStyleSheet(ButtonStyle);

	QShortcut* shortcut = new QShortcut(Qt::Key_Return,next);
	connect(shortcut, SIGNAL(activated()), next, SLOT(click()));

	QPixmap previousImg("menu/images/previous.png");
	previous->setIcon(previousImg);
	previous->setFixedSize(buttonSize);
	previous->setIconSize(buttonSize);
	previous->setFocusPolicy(Qt::NoFocus);
	previous->setFlat(true);
	previous->setStyleSheet(ButtonStyle);

	QPixmap mapImg("menu/images/map.png");
	map->setIcon(mapImg);
	map->setFixedSize(buttonSize);
	map->setIconSize(buttonSize);
	map->setFocusPolicy(Qt::NoFocus);
	map->setFlat(true);
	map->setStyleSheet(ButtonStyle);

	font = new QFont();
	font->setPixelSize(width/30);
	font->setBold(true);
	QPalette *p_ = new QPalette();
	p_->setColor(QPalette::WindowText,Qt::white);
	QPalette p = palette();

	QImage background("menu/images/background.jpg");
	QSize BackgroundSize(width,height);
	background = background.scaled(BackgroundSize,Qt::IgnoreAspectRatio,Qt::FastTransformation);
	p.setBrush(QPalette::Background, background);

	QString text_(tr("Please enter the Country Name:"));
	QFontMetrics fontMetrics(*font);
	int text_width = fontMetrics.width(text_,-1); //size of text in pixels
	int text_height = fontMetrics.height();

	text = new QLabel();
	text->setFixedWidth(text_width); //set size of QLabel acording to size of text
	text->setText(text_);
	text->setPalette(*p_);
	text->setFont(*font);

	QString countryText(tr("Country:"));
	int country_length = fontMetrics.width(countryText,-1);
	int countryLineEdit_length = width / 2;

	country = new QLabel();
	country->setFixedWidth(country_length);
	country->setText(countryText);
	country->setPalette(*p_);
	country->setFont(*font);

	countryLineEdit = new QLineEdit();
	countryLineEdit->setAlignment(Qt::AlignCenter);
	countryLineEdit->setFixedWidth(countryLineEdit_length);
	countryLineEdit->setFixedHeight(text_height);
	countryLineEdit->setFont(*font);
	countryLineEdit->grabKeyboard();
	country->setBuddy(countryLineEdit);

	QSpacerItem *horizontalTextLayoutSpacer = new QSpacerItem(width-text_width-buttonSize.width(),0);

	textLayout = new QHBoxLayout();
	textLayout->addWidget(text);
	textLayout->addSpacerItem(horizontalTextLayoutSpacer);
	textLayout->addWidget(map);

	QSpacerItem *countryLayoutSpacerLeft = new QSpacerItem((width - country_length - countryLineEdit_length)/2,0);
	QSpacerItem *countryLayoutSpacerRight = new QSpacerItem((width - country_length - countryLineEdit_length)/2,0);

	countryLayout = new QHBoxLayout();
	countryLayout->addSpacerItem(countryLayoutSpacerLeft);
	countryLayout->addWidget(country);
	countryLayout->addWidget(countryLineEdit);
	countryLayout->addSpacerItem(countryLayoutSpacerRight);

	QSpacerItem *buttonsSpacerPrevious = new QSpacerItem(0,height-buttonSize.height());

	previousButtonLayout = new QVBoxLayout();
	previousButtonLayout->setSpacing(0);
	previousButtonLayout->setMargin(0);
	previousButtonLayout->setContentsMargins(0,0,0,0);
	previousButtonLayout->addSpacerItem(buttonsSpacerPrevious);
	previousButtonLayout->addWidget(previous);

	QSpacerItem *buttonsSpacerNext = new QSpacerItem(0,height-buttonSize.height());

	nextButtonLayout = new QVBoxLayout();
	nextButtonLayout->setSpacing(0);
	nextButtonLayout->setMargin(0);
	nextButtonLayout->setContentsMargins(0,0,0,0);
	nextButtonLayout->addSpacerItem(buttonsSpacerNext);
	nextButtonLayout->addWidget(next);

	bottomLayout = new QHBoxLayout();
	bottomLayout->setSpacing(0);
	bottomLayout->setMargin(0);
	bottomLayout->setContentsMargins(0,0,0,0);
	bottomLayout->addLayout(previousButtonLayout);
	bottomLayout->addWidget(keyboard);
	bottomLayout->addLayout(nextButtonLayout);

	QSpacerItem *verticalTextLayoutSpacer = new QSpacerItem(0,30);
	QSpacerItem *spacer = new QSpacerItem(0,60);

	mainLayout = new QVBoxLayout();
	mainLayout->addLayout(textLayout);
	mainLayout->addSpacerItem(verticalTextLayoutSpacer);
	mainLayout->addLayout(countryLayout);
	mainLayout->addSpacerItem(spacer);
	mainLayout->addLayout(bottomLayout);

	connect(map,SIGNAL(clicked()),this,SIGNAL(MapClicked()));
	connect(next,SIGNAL(clicked()),this,SLOT(SetCountryName()));
	connect(next,SIGNAL(clicked()),this,SIGNAL(NextClicked()));
	connect(previous,SIGNAL(clicked()),this,SIGNAL(PreviousClicked()));

	connect(keyboard,SIGNAL(AClicked()),this,SLOT(AClicked()));
	connect(keyboard,SIGNAL(BClicked()),this,SLOT(BClicked()));
	connect(keyboard,SIGNAL(CClicked()),this,SLOT(CClicked()));
	connect(keyboard,SIGNAL(DClicked()),this,SLOT(DClicked()));
	connect(keyboard,SIGNAL(EClicked()),this,SLOT(EClicked()));
	connect(keyboard,SIGNAL(FClicked()),this,SLOT(FClicked()));
	connect(keyboard,SIGNAL(GClicked()),this,SLOT(GClicked()));
	connect(keyboard,SIGNAL(HClicked()),this,SLOT(HClicked()));
	connect(keyboard,SIGNAL(IClicked()),this,SLOT(IClicked()));
	connect(keyboard,SIGNAL(JClicked()),this,SLOT(JClicked()));
	connect(keyboard,SIGNAL(KClicked()),this,SLOT(KClicked()));
	connect(keyboard,SIGNAL(LClicked()),this,SLOT(LClicked()));
	connect(keyboard,SIGNAL(MClicked()),this,SLOT(MClicked()));
	connect(keyboard,SIGNAL(NClicked()),this,SLOT(NClicked()));
	connect(keyboard,SIGNAL(OClicked()),this,SLOT(OClicked()));
	connect(keyboard,SIGNAL(PClicked()),this,SLOT(PClicked()));
	connect(keyboard,SIGNAL(QClicked()),this,SLOT(QClicked()));
	connect(keyboard,SIGNAL(RClicked()),this,SLOT(RClicked()));
	connect(keyboard,SIGNAL(SClicked()),this,SLOT(SClicked()));
	connect(keyboard,SIGNAL(TClicked()),this,SLOT(TClicked()));
	connect(keyboard,SIGNAL(UClicked()),this,SLOT(UClicked()));
	connect(keyboard,SIGNAL(VClicked()),this,SLOT(VClicked()));
	connect(keyboard,SIGNAL(WClicked()),this,SLOT(WClicked()));
	connect(keyboard,SIGNAL(XClicked()),this,SLOT(XClicked()));
	connect(keyboard,SIGNAL(YClicked()),this,SLOT(YClicked()));
	connect(keyboard,SIGNAL(ZClicked()),this,SLOT(ZClicked()));
	connect(keyboard,SIGNAL(No0Clicked()),this,SLOT(No0Clicked()));
	connect(keyboard,SIGNAL(No1Clicked()),this,SLOT(No1Clicked()));
	connect(keyboard,SIGNAL(No2Clicked()),this,SLOT(No2Clicked()));
	connect(keyboard,SIGNAL(No3Clicked()),this,SLOT(No3Clicked()));
	connect(keyboard,SIGNAL(No4Clicked()),this,SLOT(No4Clicked()));
	connect(keyboard,SIGNAL(No5Clicked()),this,SLOT(No5Clicked()));
	connect(keyboard,SIGNAL(No6Clicked()),this,SLOT(No6Clicked()));
	connect(keyboard,SIGNAL(No7Clicked()),this,SLOT(No7Clicked()));
	connect(keyboard,SIGNAL(No8Clicked()),this,SLOT(No8Clicked()));
	connect(keyboard,SIGNAL(No9Clicked()),this,SLOT(No9Clicked()));
	connect(keyboard,SIGNAL(DelClicked()),this,SLOT(DelClicked()));
	connect(keyboard,SIGNAL(SpaceClicked()),this,SLOT(SpaceClicked()));
	connect(keyboard,SIGNAL(DashClicked()),this,SLOT(DashClicked()));



	this->setLayout(mainLayout);
	this->setPalette(p);
	this->setAutoFillBackground(true);
	this->show();
}

CSearchCountry::~CSearchCountry()
{
	delete keyboard;
}

QString
CSearchCountry::GetCountryName()
{
	return countryName;
}

void
CSearchCountry::SetCountryName()
{
	countryName = countryLineEdit->text();
}

void
CSearchCountry::AClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "A");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "a");
}

void
CSearchCountry::BClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "B");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "b");
}

void
CSearchCountry::CClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "C");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "c");
}

void
CSearchCountry::DClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "D");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "d");
}

void
CSearchCountry::EClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "E");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "e");
}

void
CSearchCountry::FClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "F");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "f");
}

void
CSearchCountry::GClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "G");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "g");
}

void
CSearchCountry::HClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "H");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "h");
}

void
CSearchCountry::IClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "I");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "i");
}

void
CSearchCountry::JClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "J");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "j");
}

void
CSearchCountry::KClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "K");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "k");
}

void
CSearchCountry::LClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "L");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "l");
}

void
CSearchCountry::MClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "M");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "m");
}

void
CSearchCountry::NClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "N");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "n");
}

void
CSearchCountry::OClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "O");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "o");
}

void
CSearchCountry::PClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "P");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "p");
}

void
CSearchCountry::QClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "Q");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "q");
}

void
CSearchCountry::RClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "R");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "r");
}

void
CSearchCountry::SClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "S");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "s");
}

void
CSearchCountry::TClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "T");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "t");
}

void
CSearchCountry::UClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "U");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "u");
}

void
CSearchCountry::VClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "V");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "v");
}

void
CSearchCountry::WClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "W");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "w");
}

void
CSearchCountry::XClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "X");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "x");
}

void
CSearchCountry::YClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "Y");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "y");
}

void
CSearchCountry::ZClicked()
{
	if (countryLineEdit->text().at(0).isNull() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isSpace() |
		countryLineEdit->text().at(countryLineEdit->cursorPosition()-1).isPunct())
		countryLineEdit->setText(countryLineEdit->text()+ "Z");
	else
		countryLineEdit->setText(countryLineEdit->text()+ "z");
}

void
CSearchCountry::No0Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "0");
}

void
CSearchCountry::No1Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "1");
}

void
CSearchCountry::No2Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "2");
}

void
CSearchCountry::No3Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "3");
}

void
CSearchCountry::No4Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "4");
}

void
CSearchCountry::No5Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "5");
}

void
CSearchCountry::No6Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "6");
}

void
CSearchCountry::No7Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "7");
}

void
CSearchCountry::No8Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "8");
}

void
CSearchCountry::No9Clicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "9");
}

void
CSearchCountry::DelClicked()
{
	countryLineEdit->backspace();
}

void
CSearchCountry::SpaceClicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ " ");
}

void
CSearchCountry::DashClicked()
{
	countryLineEdit->setText(countryLineEdit->text()+ "-");
}






