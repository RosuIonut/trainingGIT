/*
 * MenuButton.h
 *
 *  Created on: Oct 8, 2012
 *      Author: irosu
 */

#ifndef MENUBUTTON_H_
#define MENUBUTTON_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include "menu/headers/MainMenu.h"

using namespace std;
class CMainMenu;
class CMenuButton: public QWidget
{
	Q_OBJECT

public:
	explicit CMenuButton(QWidget *parent, int width_, int height_);
    ~CMenuButton();

signals:
	void MenuClicked();

private:
    QPushButton *menu;
    QPixmap menuImg;
    int width,height;
    QWidget* parent;

};

#endif /* MENUBUTTON_H_ */
