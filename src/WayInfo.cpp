/*
 * WayInfo.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: Ionut Rosu
 */

#include "WayInfo.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/**
 * Constructor for WayInfo
 */
CWayInfo::CWayInfo(){
	float a[] =   {0.0000,0.0175,0.0349,0.0524,0.0699,0.0875,0.1051,0.1228,
				   0.1405,0.1584,0.1763,0.1944,0.2125,0.2309,0.2493,0.2679,
				   0.2867,0.3057,0.3249,0.3443,0.3639,0.3838,0.4040,0.4244,
				   0.4452,0.4663,0.4877,0.5095,0.5317,0.5543,0.5773,0.6008,
				   0.6248,0.6493,0.6744,0.7001,0.7265,0.7535,0.7812,0.8097,
				   0.8390,0.8692,0.9003,0.9324,0.9656,1.0000,1.0354,1.0722,
				   1.1105,1.1502,1.1916,1.2347,1.2798,1.3269,1.3762,1.4279,
				   1.4823,1.5396,1.6001,1.6640,1.7317,1.8037,1.8804,1.9622,
				   2.0499,2.1440,2.2455,2.3553,2.4745,2.6044,2.7467,2.9033,
				   3.0767,3.2698,3.4862,3.7306,4.0091,4.3295,4.7023,5.1418,
				   5.6679,6.3095,7.1099,8.1372,9.5045,11.4157,14.2780,
				   19.0404,28.5437,56.9168};

	for(int i=0;i<90;i++)
		angleValues[i]=a[i];
}

/**
 * Destructor for WayInfo
 */
CWayInfo::~CWayInfo()
{
}

/**
 * Calculates the angle between 2 QPoints
 * @ returns angle into unsigned int
 */
int
CWayInfo::calculateAngle(QPointF pointA, QPointF pointB)
{
	qreal value = (pointB.y() - pointA.y()) / (pointB.x() - pointA.x());

	int start = 0,stop = 0;
	bool valueAccepted = false;
	int angle;

	if ((value >= 0 && value < 1 ) || (value <= 0 && value >-1))
	{
		start =  0;
		stop = 45;
		valueAccepted = true;
	}

	if ((value >= 1 && value < 2) || (value <= -1 && value >-2))
	{
		start = 45;
		stop = 64;
		valueAccepted = true;
	}

	if ((value >= 2 && value < 57) || (value <= -2 && value > -57 ))
	{
		start = 46;
		stop = 89;
		valueAccepted = true;
	}

	if (valueAccepted == true)
		if (value <= 0)
		{
			angle = angleValue(-value,start,stop);
			return -angle;
		}
		else
		{
			angle = angleValue(value,start,stop);
			return angle;
		}
	else
		if (pointA.x() > pointB.x())
			if (pointA.y() > pointB.y())
				return 90;
			else
				return -90;
		else
			if (pointA.y() > pointB.y())
				return -90;
			else
				return 90;

}

/**
 * Search for angle value in vector
 * @ returns the angle value
 */

int
CWayInfo::angleValue(qreal elem, int start, int stop)
{
	int m;
	qreal aux,aux1;
	if (start <= stop)
	{
		m = (start + stop)/2; // m = 22
		if (angleValues[m] <= elem && angleValues[m+1] >= elem)
		{
			aux = elem - angleValues[m];
			aux1 = angleValues[m+1] - elem;
			if (aux <= aux1)
				return m;
			else
				return m+1;
		}
		else
			if (angleValues[m] > elem)
				return angleValue(elem,start,m);
			else
				return angleValue(elem,m,stop);
	}
	else
		return NULL;
}
