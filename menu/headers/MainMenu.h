/*
 * MainMenu.h
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>

class CMainMenu : public QWidget
{
	Q_OBJECT

public:
	 CMainMenu(QWidget *parent, int width, int height);
     ~CMainMenu();

signals:
	void PoiClicked();
	void AddressClicked();
	void MapClicked();
	void SettingsClicked();

private:

	QWidget *parent;
	int height,width;

    QToolButton *poi,*address, *map, *settings;
    QFont *font;
    QVBoxLayout *leftButtonsLayout, *rightButtonsLayout;
    QHBoxLayout *mainLayout;

};

#endif /* MAINMENU_H_ */
