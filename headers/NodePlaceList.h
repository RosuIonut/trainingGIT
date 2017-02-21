/*
 * NodePlaceList.h
 *
 *  Created on: Sep 11, 2012
 *      Author: irosu
 */

#ifndef NODEPLACELIST_H_
#define NODEPLACELIST_H_

#include <vector>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CNodePlaceList {
private:
	vector<DT_MapNodePlaces*> m_vNodePlaces;

	vector<DT_MapNodePlaces*>::iterator m_vIterator;
public:
	CNodePlaceList();
	~CNodePlaceList();

	int getNodePlacesNo();
	void addNodePlace(DT_MapNodePlaces *NodePlace);
	DT_MapNodePlaces* getNodePlace(int NodePlaceId);
	DT_MapNodePlaces* getFirstNodePlace();
	DT_MapNodePlaces* getNextNodePlace();
	int countNodePlaces(unsigned int iNodePlaceId);

};

#endif /* NODEPLACELIST_H_ */
