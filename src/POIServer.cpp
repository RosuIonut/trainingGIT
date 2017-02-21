/*
 * POISerever.cpp
 *
 *  Created on: Apr 21, 2009
 *      Author: Ionut Rosu
 */

#include "POIServer.h"

#include "DataStructures.h"
#include "POIList.h"
#include "MapSettings.h"
#include "DataSerializers.h"

#include <QDebug>

/**
 *	Constructor for POIServer, with default values
 **/
CPOIServer::CPOIServer() :
	m_fDataFile(NULL),
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_iaBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0)
{
}

/**
 *	Get number of POIs
 *	@return number of POIs
 **/
int
CPOIServer::getPOINo()
{
	int res = 0;
	map<int, CPOIList*>::iterator iter;

	for (iter = m_vPOIs.begin(); iter != m_vPOIs.end(); iter++) {
		res += (*iter).second->getPOINo();
	}

	return (res);
}

/**
 *	Open a specified file
 *	@param	fileName - name of the file
 *	@return	0 for success, -1 for failure
 **/
int
CPOIServer::openFile(const char *fileName)
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
	printf("in POIs file we have %d horizontal bboxes and %d verical total %d\n",m_iHorBBoxes,m_iVerBBoxes, m_iTotalBBoxes);
#endif

	return 0;
}

/**
 *	Delete Bbox data with the specified number
 *	@param	iBoxNumber - the number of bbox
 *	@return 0 for success, -1 in case of error
 **/
int
CPOIServer::releaseBBoxData(int iBoxNumber)
{
	map<int, CPOIList*>::iterator iter = m_vPOIs.find(iBoxNumber);
	if (iter != m_vPOIs.end()) {
		//found,delete
		delete (*iter).second;
		m_vPOIs.erase(iter);
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
CPOIServer::loadBBoxData(int iBoxNumber)
{

#ifdef __DEBUG_NODES_SERVER_H__
	printf("CPOIServer::loadBBoxData\n");
#endif

	if (m_fDataFile == NULL) {
		printf("CPOIServer::getBBoxData() there is no file opened yet\n");
		return -1;
	}

	if ((iBoxNumber > m_iTotalBBoxes) || (iBoxNumber < 0)) {
		//bbox 0 is reserved for "no bbox"
		printf("CNodesServer::getBBoxData() bbox number is out of range: %d\n", iBoxNumber);
		return -1;
	}

	if (m_iaBBoxRepartition[iBoxNumber] == 0) {
#ifdef __DEBUG_POI_SERVER_H__
		//this is an empty bbox
		printf("empty bbox\n");
#endif
		return 0;
	}

	//bbox is ok. seek to its start
	fseek(m_fDataFile, m_llaBBoxOffsets[iBoxNumber], SEEK_SET);

	unsigned int cntPOIs = 0;
	//for each POI in bbox
	while (cntPOIs < m_iaBBoxRepartition[iBoxNumber]) {

		//try to find the bbox list. if not found add one
		map<int, CPOIList*>::iterator iter = m_vPOIs.find(iBoxNumber);
		if (iter == m_vPOIs.end()) {
			CPOIList *tmpList = new CPOIList();
			m_vPOIs[iBoxNumber] = tmpList;
			iter = m_vPOIs.find(iBoxNumber);
		}
		DT_OSMPoi *tmpPOI = new (DT_OSMPoi);
		int len, bufSize;
		if(!fread(&len, sizeof(int), 1, m_fDataFile))
			break;
		if(!fread(&bufSize, sizeof(int), 1, m_fDataFile))
			break;

		char* buf = (char*)malloc(len);
		fread(buf, len, 1, m_fDataFile);


		DS_UnserializeOSMPoi(bufSize, buf, &tmpPOI);
		free(buf);

		//add the POI element to the list
		(*iter).second->addPOI(tmpPOI);

		cntPOIs++;
	}

	//everything went fine so return 0
	return 0;
}

/**
 *	Destructor for POIServer: close all files, clean up iterator
 **/
CPOIServer::~CPOIServer()
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
	map<int, CPOIList*>::iterator iter;
	for (iter = m_vPOIs.begin(); iter != m_vPOIs.end(); iter++) {
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_vPOIs.clear();
}

/**
 *	Getter for a specified POI
 *	@param	iPOIId - the id of the POI
 *	@param	iBBoxNumber - the bbox number
 *	@return DT_OSMPoi pointer to the found POI, or NULL otherwise
 **/
DT_OSMPoi*
CPOIServer::getPOI(int iPOIId, int iBBoxNumber)
{
	DT_OSMPoi* poi = NULL;

	map<int, CPOIList*>::iterator iter = m_vPOIs.find(iBBoxNumber);
	if (iter != m_vPOIs.end()) {
		poi = (*iter).second->getPOI(iPOIId);
	}

	return poi;
}

/**
 *	Get first POI in given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_OSMPoi pointer to the found POI, NULL otherwise
 **/
DT_OSMPoi*
CPOIServer::getFirstPOI(int iBBoxNumber)
{
	DT_OSMPoi* poi = NULL;

	map<int, CPOIList*>::iterator iter = m_vPOIs.find(iBBoxNumber);
	if (iter != m_vPOIs.end()) {
		poi = ((*iter).second->getFirstPOI());
	}

	return poi;
}

/**
 *	Get next POI from given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_OSMPoi pointer to the found next POI, NULL otherwise
 **/
DT_OSMPoi*
CPOIServer::getNextPOI(int iBBoxNumber)
{
	DT_OSMPoi* poi = NULL;

	map<int, CPOIList*>::iterator iter = m_vPOIs.find(iBBoxNumber);
	if (iter != m_vPOIs.end()) {
		poi = ((*iter).second->getNextPOI());
	}

	return (poi);
}

