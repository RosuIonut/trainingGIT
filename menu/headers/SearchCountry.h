/*
 * SearchCountry.h
 *
 *  Created on: Oct 26, 2012
 *      Author: root
 */

#ifndef SEARCHCOUNTRY_H_
#define SEARCHCOUNTRY_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include "menu/headers/OnScreenKeyboard.h"

class COnScreenKeyboard;

class CSearchCountry : public QWidget
{
	Q_OBJECT

public:
	explicit CSearchCountry(QWidget *parent, int width, int height);
	~CSearchCountry();
	QString GetCountryName();

signals:
	void PreviousClicked();
	void NextClicked();
	void MapClicked();

private slots:

	void SetCountryName();
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
	QLabel *country,*text;
	QLineEdit *countryLineEdit;
	QHBoxLayout *countryLayout,*textLayout,*bottomLayout;
	QVBoxLayout *mainLayout,*previousButtonLayout, *nextButtonLayout;

	QString countryName;




};

#endif /* SEARCHCOUNTRY_H_ */
