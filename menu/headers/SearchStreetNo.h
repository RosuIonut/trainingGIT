/*
 * SearchStreetNo.h
 *
 *  Created on: Nov 6, 2012
 *      Author: Ionut Rosu
 */

#ifndef SEARCHSTREETNO_H_
#define SEARCHSTREETNO_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <menu/headers/OnScreenKeyboard.h>

class COnScreenKeyboard;

class CSearchStreetNo : public QWidget
{
	Q_OBJECT

public:
	explicit CSearchStreetNo(QWidget *parent, int width, int height);
	~CSearchStreetNo();
	QString GetStreetNo();

signals:
	void PreviousClicked();
	void NextClicked();
	void MapClicked();

private slots:

	void SetStreetNo();
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
	QLabel *streetNo,*text;
	QLineEdit *streetNoLineEdit;
	QHBoxLayout *streetNoLayout,*textLayout,*bottomLayout;
	QVBoxLayout *mainLayout,*previousButtonLayout,*nextButtonLayout;
	QPalette *p_;

	QString streetNoName;

};



#endif /* SEARCHSTREETNO_H_ */
