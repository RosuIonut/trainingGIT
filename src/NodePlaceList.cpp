/*
 * NodePlaceList.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: irosu
 */

#include "NodePlaceList.h"
/**
 *	Constructor for NodePlaceList class, iterator set to end
 **/
CNodePlaceList::CNodePlaceList(){
	m_vIterator = m_vNodePlaces.begin();
}

/**
 *	Destructor, delete iterator
 **/
CNodePlaceList::~CNodePlaceList()
{
	//clear all data
	vector<DT_MapNodePlaces*>::iterator iter;
	for (iter = m_vNodePlaces.begin(); iter != m_vNodePlaces.end(); iter++) {
		OSM_Free_NodePlaces(*iter);
	}
	m_vNodePlaces.clear();
}

/**
 *	Getter for number of NodePlaces
 *	@return number of NodePlaces
 **/

int
CNodePlaceList::getNodePlacesNo()
{
	return (m_vNodePlaces.size());
}

/**
 *	Method for adding a node
 *	@param	node - DT_MapNodePlaces pointer to the node to be added
 **/
void
CNodePlaceList::addNodePlace(DT_MapNodePlaces* NodePlace)
{
	m_vNodePlaces.push_back(NodePlace);
}

/**
 *	Getter for a node with a given id
 *	@param	nodeId - the id of the node to get
 *	@return	DT_MapNodePlaces pointer to the node if found, NULL otherwise
 **/
DT_MapNodePlaces*
CNodePlaceList::getNodePlace(int NodePlaceId)
{
	if ((unsigned int)NodePlaceId >= m_vNodePlaces.size()) {
		return NULL;
	}

	//found
	return (m_vNodePlaces.at(NodePlaceId));
}

/**
 *	Get first node
 *	@return DT_MapNodePlaces pointer to the first node, or NULL if none exist
 **/
DT_MapNodePlaces*
CNodePlaceList::getFirstNodePlace()
{
	m_vIterator = m_vNodePlaces.begin();

	if (m_vIterator == m_vNodePlaces.end()) {
		return NULL;
	}

	return (*m_vIterator);
}

/**
 *	Get next node from iterator
 *	@return	DT_MapNodePlaces pointer to the next node, NULL if at the end
 **/
DT_MapNodePlaces*
CNodePlaceList::getNextNodePlace()
{
	if (m_vIterator == m_vNodePlaces.end()) {
		return NULL;
	}

	m_vIterator++;

	if (m_vIterator == m_vNodePlaces.end()) {
		return NULL;
	}

	return (*m_vIterator);
}





