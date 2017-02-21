/*
 * NodePlaceServer.h
 *
 *  Created on: Sep 11, 2012
 *      Author: irosu
 */

#ifndef NODEPLACESERVER_H_
#define NODEPLACESERVER_H_

#include <stdio.h>
#include <map>

#include "DataStructures.h"
#include "NodePlaceList.h"

using namespace std;

class CNodePlaceServer
{

private:
	FILE*		m_fDataFile;
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	unsigned int *m_iaBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;

	map<int, map<int, CNodePlaceList*> > m_vNodePlaces;
public:
	CNodePlaceServer();
	~CNodePlaceServer();

	int	loadBBoxData(int NodePlaceNumber);
	int	releaseBBoxData(int boxNumber);
	int	openFile(const char *fileName);

	DT_MapNodePlaces* getNodePlace(int iNodePlaceId, int iBBoxNumber, int type);
	DT_MapNodePlaces* getFirstNodePlace(int iBBoxNumber, int type);
	DT_MapNodePlaces* getNextNodePlace(int iBBoxNumber, int type);

	int countNodePlaces(int iNodePlaceId, int iBBoxNumber, int type);
};


#endif /* NODEPLACESERVER_H_ */
