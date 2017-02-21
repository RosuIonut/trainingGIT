/*
 * NodePlaceServer.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: irosu
 */

#include "NodePlaceServer.h"

#include "DataStructures.h"
#include "NodePlaceList.h"
#include "MapSettings.h"
#include "DataSerializers.h"

#include <QDebug>

/**
 *	Constructor for NodePlaceServer, with default values
 **/
CNodePlaceServer::CNodePlaceServer() :
	m_fDataFile(NULL),
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_iaBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0)
{
}

/**
 *	Open a specified file
 *	@param	fileName - name of the file
 *	@return	0 for success, -1 for failure
 **/
int
CNodePlaceServer::openFile(const char *fileName)
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
	printf("in NodePlaces file we have %d horizontal bboxes and %d verical total %d\n",m_iHorBBoxes,m_iVerBBoxes, m_iTotalBBoxes);
#endif

	return 0;
}

/**
 *	Delete Bbox data with the specified number
 *	@param	iBoxNumber - the number of bbox
 *	@return 0 for success, -1 in case of error
 **/
int
CNodePlaceServer::releaseBBoxData(int iBoxNumber)
{
	map<int, map<int, CNodePlaceList*> >::iterator iter = m_vNodePlaces.find(iBoxNumber);
	if (iter != m_vNodePlaces.end())
	{
		for(map<int, CNodePlaceList*>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++)
		{
			delete (*iter2).second;
			(*iter2).second = NULL;
		}

		(*iter).second.clear();
		m_vNodePlaces.erase(iter);

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
CNodePlaceServer::loadBBoxData(int iBoxNumber)
{

#ifdef __DEBUG_NODES_SERVER_H__
	printf("CNodePlaceServer::loadBBoxData\n");
#endif

	map<int, map<int, CNodePlaceList*> >::iterator iter = m_vNodePlaces.find(iBoxNumber);
	if (iter != m_vNodePlaces.end())
		return -1;

	m_vNodePlaces[iBoxNumber]=map<int, CNodePlaceList*>();

	if (m_fDataFile == NULL) {
		printf("CNodePlaceServer::getBBoxData() there is no file opened yet\n");
		return -1;
	}

	if ((iBoxNumber > m_iTotalBBoxes) || (iBoxNumber < 0)) {
		//bbox 0 is reserved for "no bbox"
		printf("CNodePlaceServer::getBBoxData() bbox number is out of range: %d\n", iBoxNumber);
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

	unsigned int cntNodePlaces = 0;
	//for each POI in bbox
	while (cntNodePlaces < m_iaBBoxRepartition[iBoxNumber]) {

		DT_MapNodePlaces *tmpNodePlace = new (DT_MapNodePlaces);
		int len, bufSize;
		if(!fread(&len, sizeof(int), 1, m_fDataFile))
			break;
		if(!fread(&bufSize, sizeof(int), 1, m_fDataFile))
			break;

		char* buf = (char*)malloc(len);
		fread(buf, len, 1, m_fDataFile);


		DS_UnserializeOSMNodePlaces(bufSize, buf, &tmpNodePlace);
		free(buf);

		//try to find the bbox list. if not found add one
		map<int, CNodePlaceList*>::iterator iter2 = m_vNodePlaces[iBoxNumber].find(tmpNodePlace->place);

		if (iter2 == m_vNodePlaces[iBoxNumber].end()) {
			CNodePlaceList *tmpList = new CNodePlaceList();
			m_vNodePlaces[iBoxNumber][tmpNodePlace->place] = tmpList;
		}

		//add the POI element to the list
		m_vNodePlaces[iBoxNumber][tmpNodePlace->place]->addNodePlace(tmpNodePlace);

		cntNodePlaces++;
	}

	//everything went fine so return 0
	return 0;
}

/**
 *	Destructor for POIServer: close all files, clean up iterator
 **/
CNodePlaceServer::~CNodePlaceServer()
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
	for (map<int, map<int, CNodePlaceList*> >::iterator iter = m_vNodePlaces.begin(); iter != m_vNodePlaces.end(); iter++)
	{
		for(map<int, CNodePlaceList*>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++)
		{
			delete (*iter2).second;
			(*iter2).second = NULL;
		}
		(*iter).second.clear();
	}
	m_vNodePlaces.clear();
}

/**
 *	Getter for a specified NodePlace
 *	@param	iNodePlaceId - the id of the node
 *	@param	iBBoxNumber - the bbox number
 *	@return DT_MapNodePlaces pointer to the found NodePlace, or NULL otherwise
 **/
DT_MapNodePlaces*
CNodePlaceServer::getNodePlace(int iNodePlaceId, int iBBoxNumber, int type)
{
	DT_MapNodePlaces* nodePlace = NULL;

	map<int, CNodePlaceList*>::iterator iter = m_vNodePlaces[iBBoxNumber].find(type);
	if (iter != m_vNodePlaces[iBBoxNumber].end()) {
		nodePlace = ((*iter).second->getNodePlace(iNodePlaceId));
	}

	return nodePlace;
}

/**
 *	Get first NodePlace in given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_MapNodePlaces pointer to the found NodePlace, NULL otherwise
 **/
DT_MapNodePlaces*
CNodePlaceServer::getFirstNodePlace(int iBBoxNumber, int type)
{
	DT_MapNodePlaces* nodePlace = NULL;

	map<int, CNodePlaceList*>::iterator iter = m_vNodePlaces[iBBoxNumber].find(type);
	if (iter != m_vNodePlaces[iBBoxNumber].end()) {
		nodePlace = ((*iter).second->getFirstNodePlace());
	}

	return nodePlace;
}

/**
 *	Get next NodePlace from given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_MapNodePlaces pointer to the found next NodePlace, NULL otherwise
 **/
DT_MapNodePlaces*
CNodePlaceServer::getNextNodePlace(int iBBoxNumber, int type)
{
	DT_MapNodePlaces* nodePlace = NULL;

	map<int, CNodePlaceList*>::iterator iter = m_vNodePlaces[iBBoxNumber].find(type);
	if (iter != m_vNodePlaces[iBBoxNumber].end()) {
		nodePlace = ((*iter).second->getNextNodePlace());
	}

	return nodePlace;
}


