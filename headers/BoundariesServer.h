#ifndef BOUNDARIESSERVER_H_
#define BOUNDARIESSERVER_H_

#include <stdio.h>
#include <map>
#include "WaysServer.h"

extern "C" {
#include "DataStructures.h"
#include "DataSerializers.h"
}
#include "BoundariesList.h"

using namespace std;

//enum E_ServerType {
//	E_ZoomSpecific,
//	E_MaximumZoom
//};

class CBoundariesServer
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

	map<int, CBoundariesList*> m_vBBoxBoundaries;

public:
	CBoundariesServer();
	~CBoundariesServer();

	int	openFile(const char *fileName);
	int	loadBBoxData(int iBBoxNumber, int iZoomLevel);
	void releaseBBoxData(int iBBoxNumber);

	int getLastError();
	int getBoundariesNo();
	int countBoundaries(int iBoundaryId, int iBBoxNumber, int iZoomLevel);
	DT_OSMBoundaryBBox* getBoundary(int iWayId, int iBBoxNumber, int iZoomLevel, int WayNo =0);
	DT_OSMBoundaryBBox* getFirstBoundary(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
	DT_OSMBoundaryBBox* getNextBoundary(int iBBoxNumber, int iZoomLevel, E_ServerType type = E_ZoomSpecific);
};

#endif /* BOUNDARIESSERVER_H_ */
