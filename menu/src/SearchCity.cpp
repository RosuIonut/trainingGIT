/*
 * SearchCity.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: Ionut Rosu
 */

#include "menu/headers/SearchCity.h"

CSearchCity::CSearchCity(QWidget *parent_, int width_, int height_)

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

	QShortcut* nextShortcut = new QShortcut(Qt::Key_Return,next);
	connect(nextShortcut, SIGNAL(activated()), next, SLOT(click())); // Press Enter on keyboard for Next

//	QShortcut* previousShortcut = new QShortcut(Qt::Key_Backspace,previous); // shortcut for previous button using backspace
//	connect(previousShortcut, SIGNAL(activated()), previous, SLOT(click())); // either this either grabkeyboard

	QPixmap previousImg("menu/images/previous.png");
	previous->setIcon(previousImg);
	previous->setFixedSize(buttonSize);
	previous->setIconSize(buttonSize);
	previous->setFocusPolicy(Qt::NoFocus);
	previous->setFlat(true);
	previous->setStyleSheet(ButtonStyle);

	QPixmap mapImg("menu/images/map.png");
	map->setIcon(mapImg);
	map->setIconSize(buttonSize);
	map->setFixedSize(buttonSize);
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

	QString text_(tr("Please enter the City Name:"));
	QFontMetrics fontMetrics(*font);
	int text_width = fontMetrics.width(text_,-1); //size of text in pixels
	int text_height = fontMetrics.height();


	text = new QLabel();
	text->setFixedWidth(text_width); //set size of QLabel acording to size of text
	text->setText(text_);
	text->setPalette(*p_);
	text->setFont(*font);

	QString cityText(tr("City:"));
	int city_length = fontMetrics.width(cityText,-1);
	int cityLineEdit_length = width / 2;

	city = new QLabel();
	city->setFixedWidth(city_length);
	city->setText(cityText);
	city->setPalette(*p_);
	city->setFont(*font);

	cityLineEdit = new QLineEdit();
	cityLineEdit->setAlignment(Qt::AlignCenter);
	cityLineEdit->setFixedWidth(cityLineEdit_length);
	cityLineEdit->setFixedHeight(text_height);
	cityLineEdit->setFont(*font);
	cityLineEdit->grabKeyboard();
	city->setBuddy(cityLineEdit);

	QSpacerItem *horizontalTextLayoutSpacer = new QSpacerItem(width-text_width-buttonSize.width(),0);

	textLayout = new QHBoxLayout();
	textLayout->addWidget(text);
	textLayout->addSpacerItem(horizontalTextLayoutSpacer);
	textLayout->addWidget(map);

	QSpacerItem *cityLayoutSpacerLeft = new QSpacerItem((width - city_length - cityLineEdit_length)/2,0);
	QSpacerItem *cityLayoutSpacerRight = new QSpacerItem((width - city_length - cityLineEdit_length)/2,0);

	cityLayout = new QHBoxLayout();
	cityLayout->addSpacerItem(cityLayoutSpacerLeft);
	cityLayout->addWidget(city);
	cityLayout->addWidget(cityLineEdit);
	cityLayout->addSpacerItem(cityLayoutSpacerRight);

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
	mainLayout->addLayout(cityLayout);
	mainLayout->addSpacerItem(spacer);
	mainLayout->addLayout(bottomLayout);

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

	connect(map,SIGNAL(clicked()),this,SIGNAL(MapClicked()));
	connect(next,SIGNAL(clicked()),this,SLOT(SetCityName()));
	connect(next,SIGNAL(clicked()),this,SIGNAL(NextClicked()));
	connect(previous,SIGNAL(clicked()),this,SIGNAL(PreviousClicked()));

	this->setLayout(mainLayout);
	this->setPalette(p);
	this->setAutoFillBackground(true);
	this->show();

}

CSearchCity::~CSearchCity()
{
	delete keyboard;
}

QString
CSearchCity::GetCityName()
{
	return cityName;
}

void
CSearchCity::SetCityName()
{
	cityName = cityLineEdit->text();
}

void
CSearchCity::AClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "A");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "a");
}

void
CSearchCity::BClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "B");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "b");
}

void
CSearchCity::CClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "C");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "c");
}

void
CSearchCity::DClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "D");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "d");
}

void
CSearchCity::EClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "E");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "e");
}

void
CSearchCity::FClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "F");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "f");
}

void
CSearchCity::GClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "G");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "g");
}

void
CSearchCity::HClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "H");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "h");
}

void
CSearchCity::IClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "I");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "i");
}

void
CSearchCity::JClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "J");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "j");
}

void
CSearchCity::KClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "K");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "k");
}

void
CSearchCity::LClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "L");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "l");
}

void
CSearchCity::MClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "M");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "m");
}

void
CSearchCity::NClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "N");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "n");
}

void
CSearchCity::OClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "O");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "o");
}

void
CSearchCity::PClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "P");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "p");
}

void
CSearchCity::QClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "Q");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "q");
}

void
CSearchCity::RClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "R");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "r");
}

void
CSearchCity::SClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "S");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "s");
}

void
CSearchCity::TClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "T");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "t");
}

void
CSearchCity::UClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "U");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "u");
}

void
CSearchCity::VClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "V");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "v");
}

void
CSearchCity::WClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "W");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "w");
}

void
CSearchCity::XClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "X");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "x");
}

void
CSearchCity::YClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "Y");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "y");
}

void
CSearchCity::ZClicked()
{
	if (cityLineEdit->text().at(0).isNull() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isSpace() |
		cityLineEdit->text().at(cityLineEdit->cursorPosition()-1).isPunct())
		cityLineEdit->setText(cityLineEdit->text()+ "Z");
	else
		cityLineEdit->setText(cityLineEdit->text()+ "z");
}

void
CSearchCity::No0Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "0");
}

void
CSearchCity::No1Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "1");
}

void
CSearchCity::No2Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "2");
}

void
CSearchCity::No3Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "3");
}

void
CSearchCity::No4Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "4");
}

void
CSearchCity::No5Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "5");
}

void
CSearchCity::No6Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "6");
}

void
CSearchCity::No7Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "7");
}

void
CSearchCity::No8Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "8");
}

void
CSearchCity::No9Clicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "9");
}

void
CSearchCity::DelClicked()
{
	cityLineEdit->backspace();
}

void
CSearchCity::SpaceClicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ " ");
}

void
CSearchCity::DashClicked()
{
	cityLineEdit->setText(cityLineEdit->text()+ "-");
}



