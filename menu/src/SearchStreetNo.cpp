/*
 * SearchStreetNo.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: Rosu Ionut
 */

#include "menu/headers/SearchStreetNo.h"

CSearchStreetNo::CSearchStreetNo(QWidget *parent_, int width_, int height_)

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

	QShortcut *shortcut = new QShortcut(Qt::Key_Return,next);
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
	p_ = new QPalette();
	p_->setColor(QPalette::WindowText,Qt::white);
	QPalette p = palette();

	QImage background("menu/images/background.jpg");
	QSize BackgroundSize(width,height);
	background = background.scaled(BackgroundSize,Qt::IgnoreAspectRatio,Qt::FastTransformation);
	p.setBrush(QPalette::Background, background);

	QString text_(tr("Please enter the Street Number:"));
	QFontMetrics fontMetrics(*font);
	int text_width = fontMetrics.width(text_,-1); //size of text in pixels
	int text_height = fontMetrics.height();

	text = new QLabel();
	text->setFixedWidth(text_width); //set size of QLabel acording to size of text
	text->setFixedHeight(text_height);
	text->setText(text_);
	text->setPalette(*p_);
	text->setFont(*font);

	QString streetNoText(tr("Street Number:"));
	int streetNo_length = fontMetrics.width(streetNoText,-1);
	int streetNoLineEdit_length = width / 2;

	streetNo = new QLabel();
	streetNo->setFixedWidth(streetNo_length);
	streetNo->setText(streetNoText);
	streetNo->setPalette(*p_);
	streetNo->setFont(*font);

	streetNoLineEdit = new QLineEdit();
	streetNoLineEdit->setAlignment(Qt::AlignCenter);
	streetNoLineEdit->setFixedWidth(streetNoLineEdit_length);
	streetNoLineEdit->setFixedHeight(text_height);
	streetNoLineEdit->setFont(*font);
	streetNoLineEdit->grabKeyboard();
	streetNo->setBuddy(streetNoLineEdit);

	QSpacerItem *horizontalTextLayoutSpacer = new QSpacerItem(width-text_width-buttonSize.width(),0);

	textLayout = new QHBoxLayout();
	textLayout->addWidget(text);
	textLayout->addSpacerItem(horizontalTextLayoutSpacer);
	textLayout->addWidget(map);

	QSpacerItem *streetNoLayoutSpacerLeft = new QSpacerItem((width - streetNo_length - streetNoLineEdit_length)/2,0);
	QSpacerItem *streetNoLayoutSpacerRight = new QSpacerItem((width - streetNo_length - streetNoLineEdit_length)/2,0);

	streetNoLayout = new QHBoxLayout();
	streetNoLayout->addSpacerItem(streetNoLayoutSpacerLeft);
	streetNoLayout->addWidget(streetNo);
	streetNoLayout->addWidget(streetNoLineEdit);
	streetNoLayout->addSpacerItem(streetNoLayoutSpacerRight);

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
	mainLayout->addLayout(streetNoLayout);
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
	connect(next,SIGNAL(clicked()),this,SLOT(SetStreetNo()));
	connect(next,SIGNAL(clicked()),this,SIGNAL(NextClicked()));
	connect(previous,SIGNAL(clicked()),this,SIGNAL(PreviousClicked()));

	this->setLayout(mainLayout);
	this->setPalette(p);
	this->setAutoFillBackground(true);
	this->show();

}

CSearchStreetNo::~CSearchStreetNo()
{
	delete keyboard;
	delete next; delete previous; delete map;
	delete font;
	delete p_;
	delete text; delete streetNo; delete streetNoLineEdit;
	delete textLayout; delete streetNoLayout; delete previousButtonLayout;
	delete nextButtonLayout; delete bottomLayout; delete mainLayout;
//	delete shortcut;        Segmentation fault if deleted
}

QString
CSearchStreetNo::GetStreetNo()
{
	return streetNoName;
}

void
CSearchStreetNo::SetStreetNo()
{
	streetNoName = streetNoLineEdit->text();
}

void
CSearchStreetNo::AClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "A");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "a");
}

void
CSearchStreetNo::BClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "B");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "b");
}

void
CSearchStreetNo::CClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "C");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "c");
}

void
CSearchStreetNo::DClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "D");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "d");
}

void
CSearchStreetNo::EClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "E");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "e");
}

void
CSearchStreetNo::FClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "F");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "f");
}

void
CSearchStreetNo::GClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "G");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "g");
}

void
CSearchStreetNo::HClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "H");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "h");
}

void
CSearchStreetNo::IClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "I");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "i");
}

void
CSearchStreetNo::JClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "J");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "j");
}

void
CSearchStreetNo::KClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "K");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "k");
}

void
CSearchStreetNo::LClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "L");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "l");
}

void
CSearchStreetNo::MClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "M");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "m");
}

void
CSearchStreetNo::NClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "N");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "n");
}

void
CSearchStreetNo::OClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "O");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "o");
}

void
CSearchStreetNo::PClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "P");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "p");
}

void
CSearchStreetNo::QClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "Q");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "q");
}

void
CSearchStreetNo::RClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "R");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "r");
}

void
CSearchStreetNo::SClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "S");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "s");
}

void
CSearchStreetNo::TClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "T");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "t");
}

void
CSearchStreetNo::UClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "U");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "u");
}

void
CSearchStreetNo::VClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "V");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "v");
}

void
CSearchStreetNo::WClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "W");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "w");
}

void
CSearchStreetNo::XClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "X");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "x");
}

void
CSearchStreetNo::YClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "Y");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "y");
}

void
CSearchStreetNo::ZClicked()
{
	if (streetNoLineEdit->text().at(0).isNull() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isSpace() |
		streetNoLineEdit->text().at(streetNoLineEdit->cursorPosition()-1).isPunct())
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "Z");
	else
		streetNoLineEdit->setText(streetNoLineEdit->text()+ "z");
}

void
CSearchStreetNo::No0Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "0");
}

void
CSearchStreetNo::No1Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "1");
}

void
CSearchStreetNo::No2Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "2");
}

void
CSearchStreetNo::No3Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "3");
}

void
CSearchStreetNo::No4Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "4");
}

void
CSearchStreetNo::No5Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "5");
}

void
CSearchStreetNo::No6Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "6");
}

void
CSearchStreetNo::No7Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "7");
}

void
CSearchStreetNo::No8Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "8");
}

void
CSearchStreetNo::No9Clicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "9");
}

void
CSearchStreetNo::DelClicked()
{
	streetNoLineEdit->backspace();
}

void
CSearchStreetNo::SpaceClicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ " ");
}

void
CSearchStreetNo::DashClicked()
{
	streetNoLineEdit->setText(streetNoLineEdit->text()+ "-");
}


