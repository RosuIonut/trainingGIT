/*
 * TileServer.cpp
 *
 *  Created on: Apr 17, 2009
 *      Author: vlad
 */

#include "TileServer.h"

#include <QDebug>

//#define __DEBUG_TILES_SERVER_H__

/**
 *	Constructor for TileServer: open files, load bbox data
 **/
CTileServer::CTileServer()
{
	//open input files
	m_NodesServer = new CNodesServer();
	m_waysServer = new CWaysServer();
	m_areasServer = new CAreasServer();
	m_boundariesServer = new CBoundariesServer();
	m_POIServer = new CPOIServer();
	m_NodePlaceServer = new CNodePlaceServer();
	m_AddressServer = new CAddressServer();



	//nodes
	if (m_NodesServer->openFile("data/Nodes.bin") != 0) {
		printf("error while opening ways nodes file\n");
	}
	// ways
	if (m_waysServer->openFile("data/Roads.bin", "data/RoadsInfo.bin") != 0) {
		printf("error while opening ways file\n");
	}
	// areas
	if (m_areasServer->openFile("data/Areas.bin") != 0) {
		printf("error while opening areas file\n");
	}
	// boundaries
	if (m_boundariesServer->openFile("data/Boundaries.bin") != 0) {
		printf("error while opening boundaries file\n");
	}
	// pois
	if (m_POIServer->openFile("data/Pois.bin") != 0) {
		printf("error while opening pois file\n");
	}
//	 places
	if (m_NodePlaceServer->openFile("data/Places.bin") != 0) {
		printf("error while opening places file\n");
	}
//  addresses
	if (m_AddressServer->openFile("data/Addresses.bin") != 0) {
		printf("error while opening addresses file\n");
	}

	if (m_zoomLevel < 0)
		m_zoomLevel = 0;

	if (m_zoomLevel >= NO_ZOOMLEVELS)
		m_zoomLevel = NO_ZOOMLEVELS - 1;
}

/**
 *	Loads the current bbox
 *	@return operation success
 **/
int
CTileServer::LoadBBox()
{
	map<int, int>::iterator iter = m_mBboxes.find(m_bbox);
	if (iter != m_mBboxes.end())
	{
		if ((*iter).second < m_zoomLevel) {
			//load bbox ways data
			m_waysServer->loadBBoxData(m_bbox, m_zoomLevel);
			m_areasServer->loadBBoxData(m_bbox, m_zoomLevel);
			(*iter).second = m_zoomLevel;
		}
		else
			return 0;
	}
	else {
		//load bbox nodes data for testbox
		if(m_NodesServer->loadBBoxData(m_bbox) < 0)
			return -1;
		//load bbox ways data
		if(m_waysServer->loadBBoxData(m_bbox, m_zoomLevel) < 0)
			return -1;

		if(m_areasServer->loadBBoxData(m_bbox, m_zoomLevel) < 0)
			return -1;

		if(m_boundariesServer->loadBBoxData(m_bbox, m_zoomLevel) < 0)
			return -1;

		if(m_POIServer->loadBBoxData(m_bbox) < 0)
			return -1;

		if(m_NodePlaceServer->loadBBoxData(m_bbox) < 0)
			return -1;

		if(m_AddressServer->loadBBoxData(m_bbox) < 0)
			return -1;

		// store bbox and zoom level
		m_mBboxes[m_bbox] = m_zoomLevel;
	}

#ifdef __DEBUG_TILES_SERVER_H__
	printf("loaded nodes size %d\n", m_nodesServer->getNodesNo());
#endif

#ifdef __DEBUG_TILES_SERVER_H__
	printf("loaded ways size %d\n", m_waysServer->getWaysNo());
#endif

	return 0;
}

/**
 *	Get first way from TileServer
 *	@return DT_OSMWayBBox pointer to the first way
 **/
DT_OSMWayBBox*
CTileServer::getFirstWay()
{
#ifdef __DEBUG_TILES_SERVER_H__
	printf("------ TileServer.cpp::getFirstWay() -----\n");
	printf("m_bbox=%d, m_zoomLevel=%d \n", m_bbox, m_zoomLevel);
#endif
	return (m_waysServer->getFirstWay(m_bbox, m_zoomLevel));
}

/**
 *	Get next way from bbox
 *	@return pointer to the next way in bbox
 **/
DT_OSMWayBBox*
CTileServer::getNextWay()
{
	return (m_waysServer->getNextWay(m_bbox, m_zoomLevel));
}


/**
 *	Get first way from TileServer
 *	@return DT_OSMWayBBox pointer to the first way
 **/
DT_OSMAreaBBox*
CTileServer::getFirstArea()
{
	return (m_areasServer->getFirstArea(m_bbox, m_zoomLevel));
}

/**
 *	Get next way from bbox
 *	@return pointer to the next way in bbox
 **/
DT_OSMAreaBBox*
CTileServer::getNextArea()
{
	return (m_areasServer->getNextArea(m_bbox, m_zoomLevel));
}


DT_OSMBoundaryBBox*
CTileServer::getFirstBoundary()
{
	return (m_boundariesServer->getFirstBoundary(m_bbox, m_zoomLevel));
}

DT_OSMBoundaryBBox*
CTileServer::getNextBoundary()
{
	return (m_boundariesServer->getNextBoundary(m_bbox, m_zoomLevel));
}

DT_OSMPoi*
CTileServer::getFirstPOI()
{
	return (m_POIServer->getFirstPOI(m_bbox));
}

DT_OSMPoi*
CTileServer::getNextPOI()
{
	return (m_POIServer->getNextPOI(m_bbox));
}

DT_MapNodePlaces*
CTileServer::getFirstPlace(int type)
{
	return (m_NodePlaceServer->getFirstNodePlace(m_bbox, type));
}

DT_MapNodePlaces*
CTileServer::getNextPlace(int type)
{
	return (m_NodePlaceServer->getNextNodePlace(m_bbox, type));
}

DT_MapNodeAddress*
CTileServer::getFirstAddress()
{
	return (m_AddressServer->getFirstAddress(m_bbox));
}

DT_MapNodeAddress*
CTileServer::getNextAddress()
{
	return (m_AddressServer->getNextAddress(m_bbox));
}

/**
 *	Count the number of ways
 *	@param	iWayId - id of way
 *	@param	iBBoxNumber - number of bbox containing it
 *	@param	iZoomLevel - zoomlevel of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
int
CTileServer::countWays(int iWayId, int iBBoxNumber, int iZoomLevel)
{
	return m_waysServer->countWays(iWayId, iBBoxNumber, iZoomLevel);
}

int
CTileServer::countAreas(int iAreaId, int iBBoxNumber, int iZoomLevel)
{
	return m_areasServer->countAreas(iAreaId, iBBoxNumber, iZoomLevel);
}

int
CTileServer::countBoundaries(int iBoundaryId, int iBBoxNumber, int iZoomLevel)
{
	return m_boundariesServer->countBoundaries(iBoundaryId, iBBoxNumber, iZoomLevel);
}


/**
 *	Get a specified way
 *	@param	iWayId - id of way
 *	@param	iBBoxNumber - number of bbox containing it
 *	@param	iZoomLevel - zoomlevel of the way
 *	@param	iWayNo - number of way with the same id
 *	@return pointer to the way if found, NULL otherwise
 **/
DT_OSMWayBBox*
CTileServer::getWay(int iWayId, int iBBoxNumber, int iZoomLevel, int iWayNo)
{
	return m_waysServer->getWay(iWayId, iBBoxNumber, iZoomLevel, iWayNo);
}

DT_OSMAreaBBox*
CTileServer::getArea(int iAreaId, int iBBoxNumber, int iZoomLevel, int iAreaNo)
{
	return m_areasServer->getArea(iAreaId, iBBoxNumber, iZoomLevel, iAreaNo);
}

DT_OSMBoundaryBBox*
CTileServer::getBoundary(int iBoundaryId, int iBBoxNumber, int iZoomLevel, int iBoundaryNo)
{
	return m_boundariesServer->getBoundary(iBoundaryId, iBBoxNumber, iZoomLevel, iBoundaryNo);
}

/**
 *	Get node with given id
 *	@param	iNodeId - the id of the node
 *	@return	DT_BBoxNode pointer to the node
 **/
DT_BBoxNode*
CTileServer::getNode(int iNodeId)
{
	return (m_NodesServer->getNode(iNodeId, m_bbox));
}


/**
 *	Get info for a specified way
 *	@param	iBBoxNumber - box number
 *	@param	iWayId - way number
 *	@return	DT_OSMWayInfo - pointer to info
 **/
DT_OSMWayInfo*
CTileServer::getWayInfo(int iBBoxNumber, int iWayId)
{
    return (m_waysServer->getWayInfo(iBBoxNumber, iWayId));
}

/**
 *	Destructor for TileServer
 **/
CTileServer::~CTileServer()
{
	delete m_NodesServer;
	delete m_waysServer;
	delete m_areasServer;
	delete m_boundariesServer;
	delete m_POIServer;
	delete m_NodePlaceServer;
	delete m_AddressServer;
}

void CTileServer::releaseData()
{
	for(map<int, int>::iterator iter = m_mBboxes.begin(); iter != m_mBboxes.end(); iter++)
	{
		m_NodesServer->releaseBBoxData(iter->first);
		m_waysServer->releaseBBoxData(iter->first);
		m_areasServer->releaseBBoxData(iter->first);
		m_boundariesServer->releaseBBoxData(iter->first);
		m_POIServer->releaseBBoxData(iter->first);
		m_NodePlaceServer->releaseBBoxData(iter->first);
		m_AddressServer->releaseBBoxData(iter->first);
	}

	m_mBboxes.clear();
}

void CTileServer::releaseBBoxData(int bboxId)
{
	map<int, int>::iterator iter = m_mBboxes.find(bboxId);

	if (iter != m_mBboxes.end())
	{
		m_mBboxes.erase(iter);

		m_NodesServer->releaseBBoxData(bboxId);
		m_waysServer->releaseBBoxData(bboxId);
		m_areasServer->releaseBBoxData(bboxId);
		m_boundariesServer->releaseBBoxData(bboxId);
		m_POIServer->releaseBBoxData(bboxId);
		m_NodePlaceServer->releaseBBoxData(bboxId);
		m_AddressServer->releaseBBoxData(bboxId);
	}
}
