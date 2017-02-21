/*
 * POIList.h
 *
 *  Created on: Sep 3, 2012
 *      Author: Ionut Rosu
 */

#ifndef POILIST_H_
#define POILIST_H_

#include <vector>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CPOIList {
private:
	vector<DT_OSMPoi*> m_vPOIs;

	vector<DT_OSMPoi*>::iterator m_vIterator;
public:
	CPOIList();
	~CPOIList();

	int getPOINo();
	void addPOI(DT_OSMPoi *POI);
	DT_OSMPoi* getPOI(int POIId);
	DT_OSMPoi* getFirstPOI();
	DT_OSMPoi* getNextPOI();

};
#endif /* POILIST_H_ */
