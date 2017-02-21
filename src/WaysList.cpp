/*
 * WaysList.cpp
 *
 *  Created on: 28.04.2009
 *      Author: Vlad
 */

#include "WaysList.h"

/**
 *	Constructor for WaysList, set map iterator to end
 **/
CWaysList::CWaysList()
{
	m_MapIterator = m_vWays.begin();
}

/**
 *	Destructor for WaysList, delete all data
 **/
CWaysList::~CWaysList()
{
	multimap<int, DT_OSMWayBBox*>::iterator iter;

	//delete all data
	for (iter = m_vWays.begin(); iter != m_vWays.end(); iter++) {
		OSM_Way_BBox_Free((*iter).second);
	}

	m_vWays.clear();
}

/**
 *	Get number of ways
 *	@return size of ways value
 **/
int
CWaysList::getWaysNo()
{
	return (m_vWays.size());
}

/**
 *	Add a new way
 *	@param	way - DT_OSMWayBBox pointer to the way to be added
 **/
int
CWaysList::addWay(DT_OSMWayBBox* way)
{
	//add the data to the list
	m_vWays.insert(pair<int, DT_OSMWayBBox*>((int)way->id, way));

	//reset the iterator for the list to the end of the list
	m_MapIterator = m_vWays.end();

	return 0;
}

/**
 *	Counts no of ways with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
int
CWaysList::countWays(unsigned int iWayId)
{
	return m_vWays.count(iWayId);
}

/**
 *	Get way with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
DT_OSMWayBBox*
CWaysList::getWay(unsigned int iWayId, unsigned int iWayNo)
{
	multimap<int, DT_OSMWayBBox*>::iterator iter;

	iter = m_vWays.find(iWayId);
	if (iter == m_vWays.end()) {
		//the way id is not found at this zoom level
		return NULL;
	}
	while (iWayNo--) {
		iter++;
		if (iter == m_vWays.end())
			return NULL;
	}

	//we found it so we return it
	return ((*iter).second);
}

/**
 *	Get first way from ways list
 *	@return first way if exists, NULL otherwise
 **/
DT_OSMWayBBox*
CWaysList::getFirstWay()
{
	m_MapIterator = m_vWays.begin();

	if (m_MapIterator == m_vWays.end()) {
		//the map is null now
		return NULL;
	}

	//we found something so we return it
	return ((*m_MapIterator).second);
}

/**
 *	Get next way from ways list
 *	@return pointer to the next way
 **/
DT_OSMWayBBox*
CWaysList::getNextWay()
{
	if (m_MapIterator == m_vWays.end()) {
		//we are at the end or the map is empty
		return NULL;
	}

	m_MapIterator++;

	if (m_MapIterator == m_vWays.end()) {
		//we reached the end of the map
		return NULL;
	}

	//we found something so we return it
	return ((*m_MapIterator).second);
}

