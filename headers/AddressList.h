/*
 * AddressList.h
 *
 *  Created on: Oct 3, 2012
 *      Author: irosu
 */
//dummy_modif
#ifndef ADDRESSLIST_H_
#define ADDRESSLIST_H_CPCPCCP

#include <vector>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CAddressList {
private:
	vector<DT_MapNodeAddress*> m_vAddress;

	vector<DT_MapNodeAddress*>::iterator m_vIterator;
public:
	CAddressList();
	~CAddressList();

	int getAddressesNo();
	void addAddress(DT_MapNodeAddress *address);
	DT_MapNodeAddress* getAddress(int NodeAddressId);
	DT_MapNodeAddress* getFirstAddress();
	DT_MapNodeAddress* getNextAddress();
};

#endif /* ADDRESSLIST_H_ */
