/*
 * AddressServer.cpp
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */

#include "AddressServer.h"

#include "DataStructures.h"
#include "AddressList.h"
#include "MapSettings.h"
#include "DataSerializers.h"

#include <QDebug>

/**
 *	Constructor for CAddressServer, with default values
 **/
CAddressServer::CAddressServer() :
	m_fDataFile(NULL),
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_iaBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0)
{
}

/**
 *	Get number of NodeAdresses
 *	@return number of NodeAdresses
 **/
int
CAddressServer::getAddressesNo()
{
	int res = 0;
	map<int, CAddressList*>::iterator iter;

	for (iter = m_vAddress.begin(); iter != m_vAddress.end(); iter++) {
		res += (*iter).second->getAddressesNo();
	}

	return (res);
}

/**
 *	Open a specified file
 *	@param	fileName - name of the file
 *	@return	0 for success, -1 for failure
 **/
int
CAddressServer::openFile(const char *fileName)
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
	printf("in Adresses file we have %d horizontal bboxes and %d verical total %d\n",m_iHorBBoxes,m_iVerBBoxes, m_iTotalBBoxes);
#endif

	return 0;
}

/**
 *	Delete Bbox data with the specified number
 *	@param	iBoxNumber - the number of bbox
 *	@return 0 for success, -1 in case of error
 **/
int
CAddressServer::releaseBBoxData(int iBoxNumber)
{
	map<int, CAddressList*>::iterator iter = m_vAddress.find(iBoxNumber);
	if (iter != m_vAddress.end()) {
		//found,delete
		delete (*iter).second;
		m_vAddress.erase(iter);
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
CAddressServer::loadBBoxData(int iBoxNumber)
{

#ifdef __DEBUG_NODES_SERVER_H__
	printf("CAddressServer::loadBBoxData\n");
#endif

	if (m_fDataFile == NULL) {
		printf("CAddressServer::getBBoxData() there is no file opened yet\n");
		return -1;
	}

	if ((iBoxNumber > m_iTotalBBoxes) || (iBoxNumber < 0)) {
		//bbox 0 is reserved for "no bbox"
		printf("CNodesServer::getBBoxData() bbox number is out of range: %d\n", iBoxNumber);
		return -1;
	}

	if (m_iaBBoxRepartition[iBoxNumber] == 0) {
#ifdef __DEBUG_ADDRESS_SERVER_H__
		//this is an empty bbox
		printf("empty bbox\n");
#endif
		return 0;
	}

	//bbox is ok. seek to its start
	fseek(m_fDataFile, m_llaBBoxOffsets[iBoxNumber], SEEK_SET);

	unsigned int cntAddresses = 0;
	//for each NodeAddress in bbox
	while (cntAddresses < m_iaBBoxRepartition[iBoxNumber]) {

		//try to find the bbox list. if not found add one
		map<int, CAddressList*>::iterator iter = m_vAddress.find(iBoxNumber);
		if (iter == m_vAddress.end()) {
			CAddressList *tmpList = new CAddressList();
			m_vAddress[iBoxNumber] = tmpList;
			iter = m_vAddress.find(iBoxNumber);
		}
		DT_MapNodeAddress *tmpAddress = new (DT_MapNodeAddress);
		int len, bufSize;
		if(!fread(&len, sizeof(int), 1, m_fDataFile))
			break;
		if(!fread(&bufSize, sizeof(int), 1, m_fDataFile))
			break;

		char* buf = (char*)malloc(len);
		fread(buf, len, 1, m_fDataFile);


		DS_UnserializeMapNodeAddress(bufSize, buf, &tmpAddress);
		free(buf);

		//add the POI element to the list
		(*iter).second->addAddress(tmpAddress);

		cntAddresses++;
	}

	//everything went fine so return 0
	return 0;
}

/**
 *	Destructor for AddressServer: close all files, clean up iterator
 **/
CAddressServer::~CAddressServer()
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
	map<int, CAddressList*>::iterator iter;
	for (iter = m_vAddress.begin(); iter != m_vAddress.end(); iter++) {
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_vAddress.clear();
}

/**
 *	Getter for a specified NodeAddress
 *	@param	iNodeId - the id of the NodeAddress
 *	@param	iBBoxNumber - the bbox number
 *	@return DT_MapNodeAddress pointer to the found NodeAddress, or NULL otherwise
 **/
DT_MapNodeAddress*
CAddressServer::getAddress(int iNodeId, int iBBoxNumber)
{
	DT_MapNodeAddress* address = NULL;

	map<int, CAddressList*>::iterator iter = m_vAddress.find(iBBoxNumber);
	if (iter != m_vAddress.end()) {
		address = (*iter).second->getAddress(iNodeId);
	}

	return address;
}

/**
 *	Get first NodeAddress in given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_MapNodeAddress pointer to the found NodeAddress, NULL otherwise
 **/
DT_MapNodeAddress*
CAddressServer::getFirstAddress(int iBBoxNumber)
{
	DT_MapNodeAddress* address = NULL;

	map<int, CAddressList*>::iterator iter = m_vAddress.find(iBBoxNumber);
	if (iter != m_vAddress.end()) {
		address = ((*iter).second->getFirstAddress());
	}

	return address;
}

/**
 *	Get next NodeAddress from given bbox
 *	@param	iBBoxNumber - the number of bbox
 *	@return DT_MapNodeAddress pointer to the found next NodeAddress, NULL otherwise
 **/
DT_MapNodeAddress*
CAddressServer::getNextAddress(int iBBoxNumber)
{
	DT_MapNodeAddress* address = NULL;

	map<int, CAddressList*>::iterator iter = m_vAddress.find(iBBoxNumber);
	if (iter != m_vAddress.end()) {
		address = ((*iter).second->getNextAddress());
	}

	return address;
}






