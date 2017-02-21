/*
 * OnScreenKeyboard.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Rosu Ionut
 */

#ifndef ONSCREENKEYBOARD_H_
#define ONSCREENKEYBOARD_H_

#include <QtCore>
#include <QtGui>
#include <QtDebug>

class COnScreenKeyboard : public QWidget
{
	Q_OBJECT

public:
	explicit COnScreenKeyboard(QWidget *parent, int width, int height);
	~COnScreenKeyboard();
	QSize keyboardSize;

signals:
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
	QPushButton *A,*B,*C,*D,*E,*F,*G,*H,*I,*J,*K,*L,*M,*N,*O,
				*P,*Q,*R,*S,*T,*U,*V,*W,*X,*Y,*Z,*DEL,
				*no0,*no1,*no2,*no3,*no4,*no5,*no6,*no7,*no8,*no9,
				*dash,*space;
	QSpacerItem *numbersSpacingLeft,*numbersSpacingRight,*upperSpacingLeft,
				*upperSpacingRight,*middleSpacingLeft,*middleSpacingRight,
				*bottomSpacingLeft,*bottomSpacingRight;
	QFont *font1;
	QHBoxLayout *keyboardLayout1,*keyboardLayout2,*keyboardLayout3,*keyboardLayout4;
	QVBoxLayout *keyboardLayout;
	QWidget* parent;
	int width,height;
};



#endif /* ONSCREENKEYBOARD_H_ */
