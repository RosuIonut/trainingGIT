/*
 * POIList.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Rosu Ionut
 */

#include "POIList.h"
/**
 *	Constructor for POIList class, iterator set to end
 **/
CPOIList::CPOIList(){
	m_vIterator = m_vPOIs.begin();
}

/**
 *	Destructor, delete iterator
 **/
CPOIList::~CPOIList()
{
	//clear all data
	vector<DT_OSMPoi*>::iterator iter;
	for (iter = m_vPOIs.begin(); iter != m_vPOIs.end(); iter++) {
		OSM_Free_POI(*iter);
	}
	m_vPOIs.clear();
}

/**
 *	Getter for number of POIs
 *	@return number of POIs
 **/

int
CPOIList::getPOINo()
{
	return (m_vPOIs.size());
}

/**
 *	Method for adding a node
 *	@param	node - DT_BBoxNode pointer to the node to be added
 **/
void
CPOIList::addPOI(DT_OSMPoi* poi)
{
	m_vPOIs.push_back(poi);
}

/**
 *	Getter for a node with a given id
 *	@param	nodeId - the id of the node to get
 *	@return	DT_BBoxNode pointer to the node if found, NULL otherwise
 **/
DT_OSMPoi*
CPOIList::getPOI(int POIId)
{
	if ((unsigned int)POIId >= m_vPOIs.size()) {
		return NULL;
	}

	//found
	return (m_vPOIs.at(POIId));
}

/**
 *	Get first node
 *	@return DT_BBoxNode pointer to the first node, or NULL if none exist
 **/
DT_OSMPoi*
CPOIList::getFirstPOI()
{
	m_vIterator = m_vPOIs.begin();

	if (m_vIterator == m_vPOIs.end()) {
		return NULL;
	}

	return (*m_vIterator);
}

/**
 *	Get next node from iterator
 *	@return	DT_BBoxNode pointer to the next node, NULL if at the end
 **/
DT_OSMPoi*
CPOIList::getNextPOI()
{
	if (m_vIterator == m_vPOIs.end()) {
		return NULL;
	}

	m_vIterator++;

	if (m_vIterator == m_vPOIs.end()) {
		return NULL;
	}

	return (*m_vIterator);
}

