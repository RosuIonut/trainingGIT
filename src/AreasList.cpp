#include "AreasList.h"

/**
 *	Constructor for AreasList, set map iterator to end
 **/
CAreasList::CAreasList()
{
	m_MapIterator = m_vAreas.begin();
}

/**
 *	Destructor for AreasList, delete all data
 **/
CAreasList::~CAreasList()
{
	multimap<int, DT_OSMAreaBBox*>::iterator iter;

	//delete all data
	for (iter = m_vAreas.begin(); iter != m_vAreas.end(); iter++) {
		OSM_Area_BBox_Free((*iter).second);
	}

	m_vAreas.clear();
}

/**
 *	Get number of ways
 *	@return size of ways value
 **/
int
CAreasList::getAreasNo()
{
	return (m_vAreas.size());
}

/**
 *	Add a new way
 *	@param	way - DT_OSMWayBBox pointer to the way to be added
 **/
int
CAreasList::addArea(DT_OSMAreaBBox* area)
{
	//add the data to the list
	m_vAreas.insert(pair<int, DT_OSMAreaBBox*>((int)area->id, area));

	//reset the iterator for the list to the end of the list
	m_MapIterator = m_vAreas.end();

	return 0;
}

/**
 *	Counts no of ways with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
int
CAreasList::countAreas(unsigned int iAreaId)
{
	return m_vAreas.count(iAreaId);
}

/**
 *	Get way with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
DT_OSMAreaBBox*
CAreasList::getArea(unsigned int iAreaId, unsigned int iAreaNo)
{
	multimap<int, DT_OSMAreaBBox*>::iterator iter;

	iter = m_vAreas.find(iAreaId);
	if (iter == m_vAreas.end()) {
		//the way id is not found at this zoom level
		return NULL;
	}
	while (iAreaNo--) {
		iter++;
		if (iter == m_vAreas.end())
			return NULL;
	}

	//we found it so we return it
	return ((*iter).second);
}

/**
 *	Get first way from ways list
 *	@return first way if exists, NULL otherwise
 **/
DT_OSMAreaBBox*
CAreasList::getFirstArea()
{
	m_MapIterator = m_vAreas.begin();

	if (m_MapIterator == m_vAreas.end()) {
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
DT_OSMAreaBBox*
CAreasList::getNextArea()
{
	if (m_MapIterator == m_vAreas.end()) {
		//we are at the end or the map is empty
		return NULL;
	}

	m_MapIterator++;

	if (m_MapIterator == m_vAreas.end()) {
		//we reached the end of the map
		return NULL;
	}

	//we found something so we return it
	return ((*m_MapIterator).second);
}

