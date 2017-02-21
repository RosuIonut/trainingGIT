/*
 * WaysServer.h
 *
 *  Created on: Apr 21, 2009
 *      Author: vlad
 */

#ifndef WAYSSERVER_H_
#define WAYSSERVER_H_

#include <stdio.h>
#include <map>

extern "C" {
#include "DataStructures.h"
#include "DataSerializers.h"
}
#include "WaysList.h"

using namespace std;

enum E_ServerType {
	E_ZoomSpecific,
	E_MaximumZoom
};

class CWaysServer
{
private:
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	long long *m_infoBBoxOffsetTable;
	unsigned int *m_iaBBoxRepartition;
	unsigned int *m_infoBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;
	int		m_iCurZoomLevel;
	int		m_iLastError;

	FILE*		m_fDataFile;
    FILE*       m_fInfoFile;

	map<int, CWaysList*> m_vBBoxWays;
    map<int, map<int, DT_OSMWayInfo*>* > m_vBBoxWaysInfo;

public:
	CWaysServer();
	~CWaysServer();

    int	openFile(const char *fileName,const char* fileName2);
	int	loadBBoxData(int iBBoxNumber, int iZoomLevel);
	void	releaseBBoxData(int iBBoxNumber);

	int getLastError();
	int getWaysNo();
	int countWays(int iWayId, int iBBoxNumber, int iZoomLevel);
	DT_OSMWayBBox* getWay(int iWayId, int iBBoxNumber, int iZoomLevel, int WayNo =0);
	DT_OSMWayBBox* getFirstWay(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
	DT_OSMWayBBox* getNextWay(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
    DT_OSMWayInfo* getWayInfo(int iBBoxNumber, int iWayId);
};

#endif /* WAYSSERVER_H_ */
