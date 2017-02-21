/*
 * POIServer.h
 *
 *  Created on: Sep 3, 2012
 *      Author: Ionut Rosu
 */

#ifndef POISERVER_H_
#define POISERVER_H_

#include <stdio.h>
#include <map>

#include "DataStructures.h"
#include "POIList.h"

using namespace std;

class CPOIServer
{

private:
	FILE*		m_fDataFile;
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	unsigned int *m_iaBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;

	map<int, CPOIList*> m_vPOIs;
public:
	CPOIServer();
	~CPOIServer();

	int	loadBBoxData(int boxNumber);
	int	releaseBBoxData(int boxNumber);
	int	openFile(const char *fileName);

	int getPOINo();
	DT_OSMPoi* getPOI(int iPOIId, int iBBoxNumber);
	DT_OSMPoi* getFirstPOI(int iBBoxNumber);
	DT_OSMPoi* getNextPOI(int iBBoxNumber);

};


#endif /* POISERVER_H_ */
