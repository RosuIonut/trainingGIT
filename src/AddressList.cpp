/*
 * AddressList.cpp
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */

#include "AddressList.h"
/**
 *	Constructor for AddressList class, iterator set to end
 **/
CAddressList::CAddressList()
{
	m_vIterator = m_vAddress.begin();
}

/**
 *	Destructor, delete iterator
 **/
CAddressList::~CAddressList()
{
	//clear all data
	vector<DT_MapNodeAddress*>::iterator iter;
	for (iter = m_vAddress.begin(); iter != m_vAddress.end(); iter++) {
		OSM_Free_NodeAddress(*iter);
	}
	m_vAddress.clear();

}

/**
 *	Getter for number of addresses
 *	@return number of addresses
 **/
int
CAddressList::getAddressesNo()
{
	return (m_vAddress.size());
}

/**
 *	Method for adding an address
 *	@param	node - DT_MapNodeAddress pointer to the address to be added
 **/
void
CAddressList::addAddress(DT_MapNodeAddress* address)
{
	m_vAddress.push_back(address);
}

/**
 *	Getter for an address with a given id
 *	@param	nodeId - the id of the nodeaddress to get
 *	@return	DT_MapNodeAddress pointer to the node if found, NULL otherwise
 **/
DT_MapNodeAddress*
CAddressList::getAddress(int NodeAddressId)
{
	if ((unsigned int)NodeAddressId >= m_vAddress.size()) {
		return NULL;
	}

	//found
	return (m_vAddress.at(NodeAddressId));
}

/**
 *	Get first address
 *	@return DT_MapNodeAddress pointer to the first address, or NULL if none exist
 **/
DT_MapNodeAddress*
CAddressList::getFirstAddress()
{
	m_vIterator = m_vAddress.begin();

	if (m_vIterator == m_vAddress.end()) {
		return NULL;
	}

	return (*m_vIterator);
}

/**
 *	Get next address from iterator
 *	@return	DT_MapNodeAddress pointer to the next address, NULL if at the end
 **/
DT_MapNodeAddress*
CAddressList::getNextAddress()
{
	if (m_vIterator == m_vAddress.end()) {
		return NULL;
	}

	m_vIterator++;

	if (m_vIterator == m_vAddress.end()) {
		return NULL;
	}

	return (*m_vIterator);
}





