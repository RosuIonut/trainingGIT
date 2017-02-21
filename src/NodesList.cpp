/*
 * NodesList.cpp
 *
 *  Created on: Apr 30, 2009
 *      Author: Vlad
 */

#include "NodesList.h"
/**
 *	Constructor for NodesList class, iterator set to end
 **/
CNodesList::CNodesList()
{
	m_vIterator = m_vNodes.begin();
}

/**
 *	Destructor, delete iterator
 **/
CNodesList::~CNodesList()
{
	//clear all data
	vector<DT_BBoxNode*>::iterator iter;
	for (iter = m_vNodes.begin(); iter != m_vNodes.end(); iter++) {
		delete(*iter);
	}
	m_vNodes.clear();

}

/**
 *	Getter for number of nodes
 *	@return number of nodes
 **/
int
CNodesList::getNodesNo()
{
	return (m_vNodes.size());
}

/**
 *	Method for adding a node
 *	@param	node - DT_BBoxNode pointer to the node to be added
 **/
void
CNodesList::addNode(DT_BBoxNode* node)
{
	m_vNodes.push_back(node);
}

/**
 *	Getter for a node with a given id
 *	@param	nodeId - the id of the node to get
 *	@return	DT_BBoxNode pointer to the node if found, NULL otherwise
 **/
DT_BBoxNode*
CNodesList::getNode(int nodeId)
{
	if ((unsigned int)nodeId >= m_vNodes.size()) {
		return NULL;
	}

	//found
	return (m_vNodes.at(nodeId));
}

/**
 *	Get first node
 *	@return DT_BBoxNode pointer to the first node, or NULL if none exist
 **/
DT_BBoxNode*
CNodesList::getFirstNode()
{
	m_vIterator = m_vNodes.begin();

	if (m_vIterator == m_vNodes.end()) {
		return NULL;
	}

	return (*m_vIterator);
}

/**
 *	Get next node from iterator
 *	@return	DT_BBoxNode pointer to the next node, NULL if at the end
 **/
DT_BBoxNode*
CNodesList::getNextNode()
{
	if (m_vIterator == m_vNodes.end()) {
		return NULL;
	}

	m_vIterator++;

	if (m_vIterator == m_vNodes.end()) {
		return NULL;
	}

	return (*m_vIterator);
}
