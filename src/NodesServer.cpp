/*
 * NodeServer.cpp
 *
 *  Created on: Apr 21, 2009
 *      Author: vlad
 */

#include "NodesServer.h"

#include "DataStructures.h"
#include "NodesList.h"
#include "MapSettings.h"

#include <QDebug>

//#define __DEBUG_NODES_SERVER_H__

/**
 *	Constructor for NodesServer, with default values
 **/
CNodesServer::CNodesServer() :
	m_fDataFile(NULL),
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_iaBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0)
{
}

/**
 *	Get number of nodes
 *	@return number of nodes
 **/
int
CNodesServer::getNodesNo()
{
	int res = 0;
	map<int, CNodesList*>::iterator iter;
	
	for (iter = m_vBBoxNodes.begin(); iter != m_vBBoxNodes.end(); iter++) {
		res += (*iter).second->getNodesNo();
	}
	
	return (res);
}

/**
 *	Open a specified file
 *	@param	fileName - name of the file
 *	@return	0 for success, -1 for failure
 **/
int
CNodesServer::openFile(const char *fileName)
{
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
		m_fDataFile = NULL;
	}

	//open file
	m_fDataFile = fopen(fileName, "rb");
	if (m_fDataFile == NULL) {
		printf("Error while opening file\n");
		return -1;
	}

	//start reading file information
	fread(&m_iHorBBoxes, sizeof(m_iHorBBoxes), 1, m_fDataFile);
	fread(&m_iVerBBoxes, sizeof(m_iVerBBoxes), 1, m_fDataFile);

	//calculate total nr of bboxes
	m_iTotalBBoxes = m_iHorBBoxes * m_iVerBBoxes;

	//delete any previous structures
	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
		delete []m_llaBBoxOffsets;
	}

	//create structures
	m_iaBBoxRepartition = new unsigned int[m_iTotalBBoxes];
	m_llaBBoxOffsets = new unsigned long long[m_iTotalBBoxes];

	//read structures from file
	fread(m_iaBBoxRepartition, sizeof(unsigned int), m_iTotalBBoxes, m_fDataFile);
	fread(m_llaBBoxOffsets, sizeof(long long), m_iTotalBBoxes, m_fDataFile);

#ifdef __DEBUG_NODES_SERVER_H__
	printf("in nodes file we have %d horizontal bboxes and %d verical total %d\n",m_iHorBBoxes,m_iVerBBoxes, m_iTotalBBoxes);
#endif

	return 0;
}

/**
 *	Delete Bbox data with the specified number
 *	@param	iBoxNumber - the number of bbox
 *	@return 0 for success, -1 in case of error
 **/
int
CNodesServer::releaseBBoxData(int iBoxNumber)
{
	map<int, CNodesList*>::iterator iter = m_vBBoxNodes.find(iBoxNumber);
	if (iter != m_vBBoxNodes.end()) {
		//found,delete
		delete (*iter).second;
		m_vBBoxNodes.erase(iter);
		return 0;
	}

	return -1;
}

/**
 *	Load bbox data with the given number
 *	@param	iBoxNumber - the number of bbox to be loaded
 *	@return 0 for success or empty bbox, -1 for error
 **/
int
CNodesServer::loadBBoxData(int iBoxNumber)
{

#ifdef __DEBUG_NODES_SERVER_H__
	printf("CNodesServer::loadBBoxData\n");
#endif

	if (m_fDataFile == NULL) {
		printf("CNodesServer::getBBoxData() there is no file opened yet\n");
		return -1;
	}

	if ((iBoxNumber > m_iTotalBBoxes) || (iBoxNumber < 0)) {
		//bbox 0 is reserved for "no bbox"
		printf("CNodesServer::getBBoxData() bbox number is out of range: %d\n", iBoxNumber);
		return -1;
	}

	if (m_iaBBoxRepartition[iBoxNumber] == 0) {
#ifdef __DEBUG_NODES_SERVER_H__
		//this is an empty bbox
		printf("empty bbox\n");
#endif
		return 0;
	}

	//bbox is ok. seek to its start
	fseek(m_fDataFile, m_llaBBoxOffsets[iBoxNumber], SEEK_SET);

	unsigned int cntNodes = 0;
	//for each node in bbox
	while (cntNodes < m_iaBBoxRepartition[iBoxNumber]) {

		//try to find the bbox list. if not found add one
		map<int, CNodesList*>::iterator iter = m_vBBoxNodes.find(iBoxNumber);
		if (iter == m_vBBoxNodes.end()) {
			CNodesList *tmpList = new CNodesList();
			m_vBBoxNodes[(iBoxNumber)] = tmpList;
			iter = m_vBBoxNodes.find(iBoxNumber);
		}

		DT_BBoxNode *tmpNode = new (DT_BBoxNode);
		fread(tmpNode, sizeof(DT_BBoxNode), 1, m_fDataFile);
		//add the node element to the list
		(*iter).second->addNode(tmpNode);

		cntNodes++;
	}
	//everything went fine so return 0
	return 0;
}

/**
 *	Destructor for NodesServer: close all files, clean up iterator
 **/
CNodesServer::~CNodesServer()
{
	//cleanup all data
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
	}

	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
	}

	if (m_llaBBoxOffsets != NULL) {
		delete []m_llaBBoxOffsets;
	}

	//cleanup map
	map<int, CNodesList*>::iterator iter;
	for (iter = m_vBBoxNodes.begin(); iter != m_vBBoxNodes.end(); iter++) {
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_vBBoxNodes.clear();
}

/**
 *	Getter for a specified node
 *	@param	iNodeId - the id of the node 
 *	@param	iBBoxNumber - the bbox number
 *	@param	iZoomLevel - the node's zoom level
 *	@return DT_BBoxNode pointer to the found node, or NULL otherwise
 **/
DT_BBoxNode*
CNodesServer::getNode(int iNodeId, int iBBoxNumber)
{
	DT_BBoxNode* node = NULL;

	map<int, CNodesList*>::iterator iter = m_vBBoxNodes.find(iBBoxNumber);
	if (iter != m_vBBoxNodes.end()) {
		node = (*iter).second->getNode(iNodeId);
	}

	return node;
}

/**
 *	Get first node in given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@param	iZoomLevel - the node's zoom level
 *	@return DT_BBoxNode pointer to the found node, NULL otherwise
 **/
DT_BBoxNode*
CNodesServer::getFirstNode(int iBBoxNumber)
{
	DT_BBoxNode* node = NULL;

	map<int, CNodesList*>::iterator iter = m_vBBoxNodes.find(iBBoxNumber);
	if (iter != m_vBBoxNodes.end()) {
		node = ((*iter).second->getFirstNode());
	}

	return node;
}

/**
 *	Get next node from given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@param	iZoomLevel - the node's zoom level
 *	@return DT_BBoxNode pointer to the found next node, NULL otherwise
 **/
DT_BBoxNode*
CNodesServer::getNextNode(int iBBoxNumber)
{
	DT_BBoxNode* node = NULL;

	map<int, CNodesList*>::iterator iter = m_vBBoxNodes.find(iBBoxNumber);
	if (iter != m_vBBoxNodes.end()) {
		node = ((*iter).second->getNextNode());
	}

	return (node);
}

