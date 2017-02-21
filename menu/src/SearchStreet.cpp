/*
 * SearchStreet.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: Ionut Rosu
 */

#include "menu/headers/SearchStreet.h"

CSearchStreet::CSearchStreet(QWidget *parent_, int width_, int height_)

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

	QString text_(tr("Please enter the Street Name:"));
	QFontMetrics fontMetrics(*font);
	int text_width = fontMetrics.width(text_,-1); //size of text in pixels
	int text_height = fontMetrics.height();

	text = new QLabel();
	text->setFixedWidth(text_width); //set size of QLabel acording to size of text
	text->setText(text_);
	text->setPalette(*p_);
	text->setFont(*font);

	QString streetText(tr("Street:"));
	int street_length = fontMetrics.width(streetText,-1);
	int streetLineEdit_length = width / 2;

	street = new QLabel();
	street->setFixedWidth(street_length);
	street->setText(streetText);
	street->setFont(*font);
	street->setPalette(*p_);

	streetLineEdit = new QLineEdit();
	streetLineEdit->setAlignment(Qt::AlignCenter);
	streetLineEdit->setFixedWidth(streetLineEdit_length);
	streetLineEdit->setFixedHeight(text_height);
	streetLineEdit->setFont(*font);
	streetLineEdit->grabKeyboard();
	street->setBuddy(streetLineEdit);

	QSpacerItem *horizontalTextLayoutSpacer = new QSpacerItem(width-text_width-buttonSize.width(),0);

	textLayout = new QHBoxLayout();
	textLayout->addWidget(text);
	textLayout->addSpacerItem(horizontalTextLayoutSpacer);
	textLayout->addWidget(map);

	QSpacerItem *streetLayoutSpacerLeft = new QSpacerItem((width - street_length - streetLineEdit_length)/2,0);
	QSpacerItem *streetLayoutSpacerRight = new QSpacerItem((width - street_length - streetLineEdit_length)/2,0);

	streetLayout = new QHBoxLayout();
	streetLayout->addSpacerItem(streetLayoutSpacerLeft);
	streetLayout->addWidget(street);
	streetLayout->addWidget(streetLineEdit);
	streetLayout->addSpacerItem(streetLayoutSpacerRight);

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
	mainLayout->addLayout(streetLayout);
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
	connect(next,SIGNAL(clicked()),this,SLOT(SetStreetName()));
	connect(next,SIGNAL(clicked()),this,SIGNAL(NextClicked()));
	connect(previous,SIGNAL(clicked()),this,SIGNAL(PreviousClicked()));

	this->setLayout(mainLayout);
	this->setPalette(p);
	this->setAutoFillBackground(true);
	this->show();
}

CSearchStreet::~CSearchStreet()
{

}

QString
CSearchStreet::GetStreetName()
{
	return streetName;
}

void
CSearchStreet::SetStreetName()
{
	streetName = streetLineEdit->text();
}

void
CSearchStreet::AClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "A");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "a");
}

void
CSearchStreet::BClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "B");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "b");
}

void
CSearchStreet::CClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "C");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "c");
}

void
CSearchStreet::DClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "D");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "d");
}

void
CSearchStreet::EClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "E");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "e");
}

void
CSearchStreet::FClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "F");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "f");
}

void
CSearchStreet::GClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "G");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "g");
}

void
CSearchStreet::HClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "H");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "h");
}

void
CSearchStreet::IClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "I");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "i");
}

void
CSearchStreet::JClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "J");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "j");
}

void
CSearchStreet::KClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "K");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "k");
}

void
CSearchStreet::LClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "L");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "l");
}

void
CSearchStreet::MClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "M");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "m");
}

void
CSearchStreet::NClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "N");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "n");
}

void
CSearchStreet::OClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "O");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "o");
}

void
CSearchStreet::PClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "P");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "p");
}

void
CSearchStreet::QClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "Q");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "q");
}

void
CSearchStreet::RClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "R");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "r");
}

void
CSearchStreet::SClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "S");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "s");
}

void
CSearchStreet::TClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "T");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "t");
}

void
CSearchStreet::UClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "U");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "u");
}

void
CSearchStreet::VClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "V");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "v");
}

void
CSearchStreet::WClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "W");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "w");
}

void
CSearchStreet::XClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "X");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "x");
}

void
CSearchStreet::YClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "Y");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "y");
}

void
CSearchStreet::ZClicked()
{
	if (streetLineEdit->text().at(0).isNull() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isSpace() |
		streetLineEdit->text().at(streetLineEdit->cursorPosition()-1).isPunct())
		streetLineEdit->setText(streetLineEdit->text()+ "Z");
	else
		streetLineEdit->setText(streetLineEdit->text()+ "z");
}

void
CSearchStreet::No0Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "0");
}

void
CSearchStreet::No1Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "1");
}

void
CSearchStreet::No2Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "2");
}

void
CSearchStreet::No3Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "3");
}

void
CSearchStreet::No4Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "4");
}

void
CSearchStreet::No5Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "5");
}

void
CSearchStreet::No6Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "6");
}

void
CSearchStreet::No7Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "7");
}

void
CSearchStreet::No8Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "8");
}

void
CSearchStreet::No9Clicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "9");
}

void
CSearchStreet::DelClicked()
{
	streetLineEdit->backspace();
}

void
CSearchStreet::SpaceClicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ " ");
}

void
CSearchStreet::DashClicked()
{
	streetLineEdit->setText(streetLineEdit->text()+ "-");
}




