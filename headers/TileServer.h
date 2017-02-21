/*
 * TileServer.h
 *
 *  Created on: Apr 17, 2009
 *      Author: vlad
 */

#ifndef TILESERVER_H_
#define TILESERVER_H_

#include "NodesServer.h"
#include "WaysServer.h"
#include "BoundariesServer.h"
#include "AreasServer.h"
#include "POIServer.h"
#include "NodePlaceServer.h"
#include "MapSettings.h"
#include "AddressServer.h"

class CTileServer
{
private:
	CNodesServer* m_NodesServer;
	CWaysServer* m_waysServer;
	CAreasServer* m_areasServer;
	CBoundariesServer* m_boundariesServer;
	CPOIServer* m_POIServer;
	CNodePlaceServer* m_NodePlaceServer;
	CAddressServer* m_AddressServer;

	int m_bbox;
	int m_zoomLevel;

	map<int, int> m_mBboxes;

public:
	CTileServer();
	~CTileServer();

	// inline
	inline void setBBox(int bbox) { m_bbox = bbox;}

	inline void setZoomLevel(int iZoomLevel) {
		m_zoomLevel = iZoomLevel;

	if (m_zoomLevel < 0)
		m_zoomLevel = 0;

	if (m_zoomLevel >= NO_ZOOMLEVELS)
		m_zoomLevel = NO_ZOOMLEVELS - 1;}

	int LoadBBox();

	DT_OSMWayBBox* getFirstWay();
	DT_OSMWayBBox* getNextWay();

	DT_OSMAreaBBox* getFirstArea();
	DT_OSMAreaBBox* getNextArea();

	DT_OSMBoundaryBBox* getFirstBoundary();
	DT_OSMBoundaryBBox* getNextBoundary();

	DT_OSMPoi* getFirstPOI();
	DT_OSMPoi* getNextPOI();

	DT_MapNodePlaces* getFirstPlace(int);
	DT_MapNodePlaces* getNextPlace(int);

	DT_MapNodeAddress* getFirstAddress();
	DT_MapNodeAddress* getNextAddress();


	DT_BBoxNode* getNode(int iNodeId);


	int countWays(int iWayId, int iBBoxNumber, int iZoomLevel);
	int countAreas(int iAreaId, int iBBoxNumber, int iZoomLevel);
	int countBoundaries(int iBoundaryId, int iBBoxNumber, int iZoomLevel);

	DT_OSMWayBBox* getWay(int iWayId, int iBBoxNumber, int iZoomLevel, int iWayNo);
	DT_OSMAreaBBox* getArea(int iAreaId, int iBBoxNumber, int iZoomLevel, int iAreaNo);
	DT_OSMBoundaryBBox* getBoundary(int iBoundaryId, int iBBoxNumber, int iZoomLevel, int iBoundaryNo);
    DT_OSMWayInfo* getWayInfo(int iBBoxNumber, int iWayId);

	CNodesServer* getNodeServer(){ return m_NodesServer; }

	void releaseBBoxData(int bboxId);
	void releaseData();
};

#endif /* TILESERVER_H_ */
