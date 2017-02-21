/*
 * OnScreenKeyboard.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Rosu Ionut
 */

#include <menu/headers/OnScreenKeyboard.h>


COnScreenKeyboard::COnScreenKeyboard(QWidget *parent_,int width_, int height_)

	:QWidget(parent_)
{

	parent = parent_;
	width = width_;
	height = height_;

	QString buttonStyle =   "QPushButton {"
							"font-weight: bold;"
							"margin: 0 1px 0 1px;"
							"color: black;"
							"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		                                "stop: 0 #E6E6E6, stop: 1 #9C9C9C);"
							"border-style: inset;"
							"border-radius: 5px;"
							"border-width: 0px;"
							"border-color: #676673;"
							"}"

							"QPushButton:pressed {"
							"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		                                      "stop: 0 #9C9C9C, stop: 1 #E6E6E6);"
							"}"
							"QPushButton:hover {"
							"border-width: 3px;"
							"}";

	QSize keySize(width/15,width/15);
	QSize specialKeySize(width/15+width/20,width/15);
	QSize spaceKeySize(width/15+width/5,width/15);

	QSize numbersKeyboardSize(10 * keySize.width()+specialKeySize.width(),keySize.height());
	QSize upperKeyboardSize(10 * keySize.width(),keySize.height());
	QSize middleKeyboardSize(10 * keySize.width(),keySize.height());
	QSize bottomKeyboardSize(7 * keySize.width() + spaceKeySize.width(),keySize.height());

	keyboardSize.setHeight(4*keySize.height());
	keyboardSize.setWidth(numbersKeyboardSize.width());
	this->resize(keyboardSize);

	numbersSpacingLeft = new QSpacerItem((keyboardSize.width()-numbersKeyboardSize.width())/2,0);
	numbersSpacingRight = new QSpacerItem((keyboardSize.width()-numbersKeyboardSize.width())/2,0);

	upperSpacingLeft = new QSpacerItem((keyboardSize.width()-upperKeyboardSize.width())/2,0);
	upperSpacingRight = new QSpacerItem((keyboardSize.width()-upperKeyboardSize.width())/2,0);

	middleSpacingLeft = new QSpacerItem((keyboardSize.width()-middleKeyboardSize.width())/2,0);
	middleSpacingRight = new QSpacerItem((keyboardSize.width()-middleKeyboardSize.width())/2,0);

	bottomSpacingLeft = new QSpacerItem((keyboardSize.width()-bottomKeyboardSize.width())/2,0);
	bottomSpacingRight = new QSpacerItem((keyboardSize.width()-bottomKeyboardSize.width())/2,0);

	font1 = new QFont();
	font1->setPixelSize(keySize.width()/1.5);

	A = new QPushButton("A"); B = new QPushButton("B"); C = new QPushButton("C");
	D = new QPushButton("D"); E = new QPushButton("E"); F = new QPushButton("F");
	G = new QPushButton("G"); H = new QPushButton("H"); I = new QPushButton("I");
	J = new QPushButton("J"); K = new QPushButton("K"); L = new QPushButton("L");
	M = new QPushButton("M"); N = new QPushButton("N"); O = new QPushButton("O");
	P = new QPushButton("P"); Q = new QPushButton("Q"); R = new QPushButton("R");
	S = new QPushButton("S"); T = new QPushButton("T"); U = new QPushButton("U");
	V = new QPushButton("V"); W = new QPushButton("W"); X = new QPushButton("X");
	Y = new QPushButton("Y"); Z = new QPushButton("Z"); DEL = new QPushButton("<-");
	no1 = new QPushButton("1"); no2 = new QPushButton("2");	no3 = new QPushButton("3");
	no4 = new QPushButton("4"); no5 = new QPushButton("5");	no6 = new QPushButton("6");
	no7 = new QPushButton("7"); no8 = new QPushButton("8");	no9 = new QPushButton("9");
	no0 = new QPushButton("0"); dash = new QPushButton("-"); space = new QPushButton();

	DEL->setAutoRepeat(true);

	A->setStyleSheet(buttonStyle); B->setStyleSheet(buttonStyle); C->setStyleSheet(buttonStyle);
	D->setStyleSheet(buttonStyle); E->setStyleSheet(buttonStyle); F->setStyleSheet(buttonStyle);
	G->setStyleSheet(buttonStyle); H->setStyleSheet(buttonStyle); I->setStyleSheet(buttonStyle);
	J->setStyleSheet(buttonStyle); K->setStyleSheet(buttonStyle); L->setStyleSheet(buttonStyle);
	M->setStyleSheet(buttonStyle); N->setStyleSheet(buttonStyle); O->setStyleSheet(buttonStyle);
	P->setStyleSheet(buttonStyle); Q->setStyleSheet(buttonStyle); R->setStyleSheet(buttonStyle);
	S->setStyleSheet(buttonStyle); T->setStyleSheet(buttonStyle); U->setStyleSheet(buttonStyle);
	V->setStyleSheet(buttonStyle); W->setStyleSheet(buttonStyle); X->setStyleSheet(buttonStyle);
	Y->setStyleSheet(buttonStyle); Z->setStyleSheet(buttonStyle); DEL->setStyleSheet(buttonStyle);
	no0->setStyleSheet(buttonStyle); no1->setStyleSheet(buttonStyle); no2->setStyleSheet(buttonStyle);
	no3->setStyleSheet(buttonStyle); no4->setStyleSheet(buttonStyle); no5->setStyleSheet(buttonStyle);
	no6->setStyleSheet(buttonStyle); no7->setStyleSheet(buttonStyle); no8->setStyleSheet(buttonStyle);
	no9->setStyleSheet(buttonStyle); dash->setStyleSheet(buttonStyle); space->setStyleSheet(buttonStyle);

	A->setFont(*font1); B->setFont(*font1); C->setFont(*font1); D->setFont(*font1); E->setFont(*font1);
	F->setFont(*font1); G->setFont(*font1); H->setFont(*font1); I->setFont(*font1); J->setFont(*font1);
	K->setFont(*font1); L->setFont(*font1); M->setFont(*font1); N->setFont(*font1); O->setFont(*font1);
	P->setFont(*font1); Q->setFont(*font1); R->setFont(*font1); S->setFont(*font1); T->setFont(*font1);
	U->setFont(*font1); V->setFont(*font1); W->setFont(*font1); X->setFont(*font1); Y->setFont(*font1);
	Z->setFont(*font1); DEL->setFont(*font1); no0->setFont(*font1); no1->setFont(*font1);
	no2->setFont(*font1); no3->setFont(*font1); no4->setFont(*font1); no5->setFont(*font1);
	no6->setFont(*font1); no7->setFont(*font1); no8->setFont(*font1);no9->setFont(*font1);
	dash->setFont(*font1); space->setFont(*font1);

	A->setFocusPolicy(Qt::NoFocus); B->setFocusPolicy(Qt::NoFocus); C->setFocusPolicy(Qt::NoFocus);
	D->setFocusPolicy(Qt::NoFocus); E->setFocusPolicy(Qt::NoFocus); F->setFocusPolicy(Qt::NoFocus);
	G->setFocusPolicy(Qt::NoFocus); H->setFocusPolicy(Qt::NoFocus); I->setFocusPolicy(Qt::NoFocus);
	J->setFocusPolicy(Qt::NoFocus); K->setFocusPolicy(Qt::NoFocus); L->setFocusPolicy(Qt::NoFocus);
	M->setFocusPolicy(Qt::NoFocus); N->setFocusPolicy(Qt::NoFocus); O->setFocusPolicy(Qt::NoFocus);
	P->setFocusPolicy(Qt::NoFocus); Q->setFocusPolicy(Qt::NoFocus); R->setFocusPolicy(Qt::NoFocus);
	S->setFocusPolicy(Qt::NoFocus); T->setFocusPolicy(Qt::NoFocus); U->setFocusPolicy(Qt::NoFocus);
	V->setFocusPolicy(Qt::NoFocus); W->setFocusPolicy(Qt::NoFocus); X->setFocusPolicy(Qt::NoFocus);
	Y->setFocusPolicy(Qt::NoFocus); Z->setFocusPolicy(Qt::NoFocus); DEL->setFocusPolicy(Qt::NoFocus);
	no0->setFocusPolicy(Qt::NoFocus); no1->setFocusPolicy(Qt::NoFocus); no2->setFocusPolicy(Qt::NoFocus);
	no3->setFocusPolicy(Qt::NoFocus); no4->setFocusPolicy(Qt::NoFocus);	no5->setFocusPolicy(Qt::NoFocus);
	no6->setFocusPolicy(Qt::NoFocus); no7->setFocusPolicy(Qt::NoFocus);	no8->setFocusPolicy(Qt::NoFocus);
	no9->setFocusPolicy(Qt::NoFocus); dash->setFocusPolicy(Qt::NoFocus); space->setFocusPolicy(Qt::NoFocus);

	A->setFixedSize(keySize); B->setFixedSize(keySize); C->setFixedSize(keySize);
	D->setFixedSize(keySize); E->setFixedSize(keySize); F->setFixedSize(keySize);
	G->setFixedSize(keySize); H->setFixedSize(keySize); I->setFixedSize(keySize);
	J->setFixedSize(keySize); K->setFixedSize(keySize); L->setFixedSize(keySize);
	M->setFixedSize(keySize); N->setFixedSize(keySize); O->setFixedSize(keySize);
	P->setFixedSize(keySize); Q->setFixedSize(keySize); R->setFixedSize(keySize);
	S->setFixedSize(keySize); T->setFixedSize(keySize); U->setFixedSize(keySize);
	V->setFixedSize(keySize); W->setFixedSize(keySize); X->setFixedSize(keySize);
	Y->setFixedSize(keySize); Z->setFixedSize(keySize); no0->setFixedSize(keySize);
	no1->setFixedSize(keySize); no2->setFixedSize(keySize); no3->setFixedSize(keySize);
	no4->setFixedSize(keySize); no5->setFixedSize(keySize); no6->setFixedSize(keySize);
	no7->setFixedSize(keySize); no8->setFixedSize(keySize); no9->setFixedSize(keySize);
	dash->setFixedSize(keySize);

	DEL->setFixedSize(specialKeySize);
	space->setFixedSize(spaceKeySize);

	keyboardLayout1 = new QHBoxLayout();
	keyboardLayout1->setSpacing(0);
	keyboardLayout1->setMargin(0);
	keyboardLayout1->setContentsMargins(0,0,0,0);
	keyboardLayout1->addSpacerItem(upperSpacingLeft);
	keyboardLayout1->addWidget(Q); keyboardLayout1->addWidget(W); keyboardLayout1->addWidget(E);
	keyboardLayout1->addWidget(R); keyboardLayout1->addWidget(T); keyboardLayout1->addWidget(Y);
	keyboardLayout1->addWidget(U); keyboardLayout1->addWidget(I); keyboardLayout1->addWidget(O);
	keyboardLayout1->addWidget(P);
	keyboardLayout1->addSpacerItem(upperSpacingRight);

	keyboardLayout2 = new QHBoxLayout();
	keyboardLayout2->setSpacing(0);
	keyboardLayout2->setMargin(0);
	keyboardLayout2->setContentsMargins(0,0,0,0);
	keyboardLayout2->addSpacerItem(middleSpacingLeft);
	keyboardLayout2->addWidget(A); keyboardLayout2->addWidget(S); keyboardLayout2->addWidget(D);
	keyboardLayout2->addWidget(F); keyboardLayout2->addWidget(G); keyboardLayout2->addWidget(H);
	keyboardLayout2->addWidget(J); keyboardLayout2->addWidget(K); keyboardLayout2->addWidget(L);
	keyboardLayout2->addWidget(dash);
	keyboardLayout2->addSpacerItem(middleSpacingRight);

	keyboardLayout3 = new QHBoxLayout();
	keyboardLayout3->setSpacing(0);
	keyboardLayout3->setMargin(0);
	keyboardLayout3->setContentsMargins(0,0,0,0);
	keyboardLayout3->addSpacerItem(bottomSpacingLeft);
	keyboardLayout3->addWidget(Z); keyboardLayout3->addWidget(X); keyboardLayout3->addWidget(C);
	keyboardLayout3->addWidget(V); keyboardLayout3->addWidget(space);keyboardLayout3->addWidget(B); keyboardLayout3->addWidget(N);
	keyboardLayout3->addWidget(M);
	keyboardLayout3->addSpacerItem(bottomSpacingRight);

	keyboardLayout4 = new QHBoxLayout();
	keyboardLayout4->setSpacing(0);
	keyboardLayout4->setMargin(0);
	keyboardLayout4->setContentsMargins(0,0,0,0);
	keyboardLayout4->addSpacerItem(numbersSpacingLeft);
	keyboardLayout4->addWidget(no0); keyboardLayout4->addWidget(no1); keyboardLayout4->addWidget(no2);
	keyboardLayout4->addWidget(no3); keyboardLayout4->addWidget(no4); keyboardLayout4->addWidget(no5);
	keyboardLayout4->addWidget(no6); keyboardLayout4->addWidget(no7); keyboardLayout4->addWidget(no8);
	keyboardLayout4->addWidget(no9); keyboardLayout4->addWidget(DEL);
	keyboardLayout4->addSpacerItem(numbersSpacingRight);

	keyboardLayout = new QVBoxLayout();
	keyboardLayout->setSpacing(0);
	keyboardLayout->setMargin(0);
	keyboardLayout->setContentsMargins(0,0,0,0);
	keyboardLayout->addLayout(keyboardLayout4);
	keyboardLayout->addLayout(keyboardLayout1);
	keyboardLayout->addLayout(keyboardLayout2);
	keyboardLayout->addLayout(keyboardLayout3);

	connect(A,SIGNAL(clicked()),this,SIGNAL(AClicked()));
	connect(B,SIGNAL(clicked()),this,SIGNAL(BClicked()));
	connect(C,SIGNAL(clicked()),this,SIGNAL(CClicked()));
	connect(D,SIGNAL(clicked()),this,SIGNAL(DClicked()));
	connect(E,SIGNAL(clicked()),this,SIGNAL(EClicked()));
	connect(F,SIGNAL(clicked()),this,SIGNAL(FClicked()));
	connect(G,SIGNAL(clicked()),this,SIGNAL(GClicked()));
	connect(H,SIGNAL(clicked()),this,SIGNAL(HClicked()));
	connect(I,SIGNAL(clicked()),this,SIGNAL(IClicked()));
	connect(J,SIGNAL(clicked()),this,SIGNAL(JClicked()));
	connect(K,SIGNAL(clicked()),this,SIGNAL(KClicked()));
	connect(L,SIGNAL(clicked()),this,SIGNAL(LClicked()));
	connect(M,SIGNAL(clicked()),this,SIGNAL(MClicked()));
	connect(N,SIGNAL(clicked()),this,SIGNAL(NClicked()));
	connect(O,SIGNAL(clicked()),this,SIGNAL(OClicked()));
	connect(P,SIGNAL(clicked()),this,SIGNAL(PClicked()));
	connect(Q,SIGNAL(clicked()),this,SIGNAL(QClicked()));
	connect(R,SIGNAL(clicked()),this,SIGNAL(RClicked()));
	connect(S,SIGNAL(clicked()),this,SIGNAL(SClicked()));
	connect(T,SIGNAL(clicked()),this,SIGNAL(TClicked()));
	connect(U,SIGNAL(clicked()),this,SIGNAL(UClicked()));
	connect(V,SIGNAL(clicked()),this,SIGNAL(VClicked()));
	connect(W,SIGNAL(clicked()),this,SIGNAL(WClicked()));
	connect(X,SIGNAL(clicked()),this,SIGNAL(XClicked()));
	connect(Y,SIGNAL(clicked()),this,SIGNAL(YClicked()));
	connect(Z,SIGNAL(clicked()),this,SIGNAL(ZClicked()));
	connect(dash,SIGNAL(clicked()),this,SIGNAL(DashClicked()));

	connect(no0,SIGNAL(clicked()),this,SIGNAL(No0Clicked()));
	connect(no1,SIGNAL(clicked()),this,SIGNAL(No1Clicked()));
	connect(no2,SIGNAL(clicked()),this,SIGNAL(No2Clicked()));
	connect(no3,SIGNAL(clicked()),this,SIGNAL(No3Clicked()));
	connect(no4,SIGNAL(clicked()),this,SIGNAL(No4Clicked()));
	connect(no5,SIGNAL(clicked()),this,SIGNAL(No5Clicked()));
	connect(no6,SIGNAL(clicked()),this,SIGNAL(No6Clicked()));
	connect(no7,SIGNAL(clicked()),this,SIGNAL(No7Clicked()));
	connect(no8,SIGNAL(clicked()),this,SIGNAL(No8Clicked()));
	connect(no9,SIGNAL(clicked()),this,SIGNAL(No9Clicked()));

	connect(DEL,SIGNAL(clicked()),this,SIGNAL(DelClicked()));
	connect(space,SIGNAL(clicked()),this,SIGNAL(SpaceClicked()));


	this->setLayout(keyboardLayout);
	this->show();


}

COnScreenKeyboard::~COnScreenKeyboard()
{
	delete font1;

	delete A; delete B; delete C; delete D; delete E; delete F;
	delete G; delete H; delete I; delete J; delete K; delete L;
	delete M; delete N; delete O; delete P; delete Q; delete R;
	delete S; delete T; delete U; delete V; delete W; delete X;
	delete Y; delete Z; delete no0; delete no1; delete no2;
	delete no3; delete no4; delete no5; delete no6; delete no7;          // Keep this order
	delete no8; delete no9; delete DEL; delete dash; delete space;

	delete keyboardLayout1; delete keyboardLayout2;
	delete keyboardLayout3; delete keyboardLayout4;
	delete keyboardLayout;

/*	delete numbersSpacingLeft;delete numbersSpacingRight;
	delete upperSpacingLeft;  delete upperSpacingRight;
	delete middleSpacingLeft; delete middleSpacingRight;
	delete bottomSpacingLeft; delete bottomSpacingRight;   Segmentation fault if deleted */
}





