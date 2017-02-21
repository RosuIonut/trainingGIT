#ifndef AREASSERVER_H_
#define AREASSERVER_H_

#include <stdio.h>
#include <map>
#include "WaysServer.h"

extern "C" {
#include "DataStructures.h"
#include "DataSerializers.h"
}
#include "AreasList.h"

using namespace std;

//enum E_ServerType {
//	E_ZoomSpecific,
//	E_MaximumZoom
//};

class CAreasServer
{
private:
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	unsigned int *m_iaBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;
	int		m_iCurZoomLevel;
	int		m_iLastError;

	FILE*		m_fDataFile;

	map<int, CAreasList*> m_vBBoxAreas;

public:
	CAreasServer();
	~CAreasServer();

	int	openFile(const char *fileName);
	int	loadBBoxData(int iBBoxNumber, int iZoomLevel);
	void releaseBBoxData(int iBBoxNumber);

	int getLastError();
	int getAreasNo();
	int countAreas(int iAreaId, int iBBoxNumber, int iZoomLevel);
	DT_OSMAreaBBox* getArea(int iWayId, int iBBoxNumber, int iZoomLevel, int WayNo =0);
	DT_OSMAreaBBox* getFirstArea(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
	DT_OSMAreaBBox* getNextArea(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
};

#endif /* AREASSERVER_H_ */
