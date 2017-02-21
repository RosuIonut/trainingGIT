/*
 * MenuButton.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: irosu
 */

#include "menu/headers/MenuButton.h"

CMenuButton::CMenuButton(QWidget *parent_, int width_, int height_)
	:QWidget(parent_)
{
	parent = parent_;
	width = width_;
	height = height_;
	menu = new QPushButton();
	QSize buttonSize(width/13,width/13);
	this->resize(width,height);

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

	QPixmap menuImg("menu/images/main_menu.png");
	menu->setFixedSize(buttonSize);
	menu->setIconSize(buttonSize);
	menu->setIcon(menuImg);
	menu->setFocusPolicy(Qt::NoFocus);
	menu->setFlat(true);
	menu->setStyleSheet(ButtonStyle);

	connect(menu,SIGNAL(clicked()),this,SIGNAL(MenuClicked()));

	QSpacerItem *spacer = new QSpacerItem(0,height-buttonSize.height());

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(menu);
	layout->addSpacerItem(spacer);

	this->setLayout(layout);
	this->show();

}

CMenuButton::~CMenuButton()
{

}




