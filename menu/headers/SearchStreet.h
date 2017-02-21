/*
 * SearchStreet.h
 *
 *  Created on: Nov 6, 2012
 *      Author: Ionut Rosu
 */

#ifndef SEARCHSTREET_H_
#define SEARCHSTREET_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <menu/headers/OnScreenKeyboard.h>

class COnScreenKeyboard;

class CSearchStreet : public QWidget
{
	Q_OBJECT

public:
	explicit CSearchStreet(QWidget *parent, int width, int height);
	~CSearchStreet();
	QString GetStreetName();

signals:
	void PreviousClicked();
	void NextClicked();
	void MapClicked();

private slots:

	void SetStreetName();
	void AClicked(); void BClicked(); void CClicked(); void DClicked();
	void EClicked(); void FClicked(); void GClicked(); void HClicked();
	void IClicked(); void JClicked(); void KClicked(); void LClicked();
	void MClicked(); void NClicked(); void OClicked(); void PClicked();
	void QClicked(); void RClicked(); void SClicked(); void TClicked();
	void UClicked(); void VClicked(); void WClicked(); void XClicked();
	void YClicked(); void ZClicked(); void No0Clicked(); void No1Clicked();
	void No2Clicked(); void No3Clicked(); void No4Clicked(); void No5Clicked();
	void No6Clicked(); void No7Clicked(); void No8Clicked(); void No9Clicked();
	void DelClicked(); void SpaceClicked(); void DashClicked();

private:

	QWidget* parent;
	int width,height;

	COnScreenKeyboard *keyboard;
	QPushButton *previous,*next,*map;
	QFont *font;
	QLabel *street,*text;
	QLineEdit *streetLineEdit;

	QHBoxLayout *streetLayout,*textLayout,*bottomLayout;
	QVBoxLayout *mainLayout,*previousButtonLayout,*nextButtonLayout;

	QString streetName;

};


#endif /* SEARCHSTREET_H_ */
