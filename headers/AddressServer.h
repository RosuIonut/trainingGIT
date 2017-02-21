/*
 * AddressServer.h
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */

#ifndef ADDRESSSERVER_H_
#define ADDRESSSERVER_H_

#include <stdio.h>
#include <map>

#include "DataStructures.h"
#include "AddressList.h"

using namespace std;

class CAddressServer
{

private:
	FILE*		m_fDataFile;
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	unsigned int *m_iaBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;

	map<int, CAddressList*> m_vAddress;
public:
	CAddressServer();
	~CAddressServer();

	int	loadBBoxData(int boxNumber);
	int	releaseBBoxData(int boxNumber);
	int	openFile(const char *fileName);

	int getAddressesNo();
	DT_MapNodeAddress* getAddress(int iNodeId, int iBBoxNumber);
	DT_MapNodeAddress* getFirstAddress(int iBBoxNumber);
	DT_MapNodeAddress* getNextAddress(int iBBoxNumber);
};

#endif /* ADDRESSSERVER_H_ */
