#ifndef WAYSLIST_H_
#define WAYSLIST_H_

#include <map>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CWaysList {
private:
	multimap<int, DT_OSMWayBBox*> m_vWays;

	//the map iterator
	multimap<int, DT_OSMWayBBox*>::iterator m_MapIterator;
public:
	CWaysList();
	~CWaysList();

	int getWaysNo();
	int addWay(DT_OSMWayBBox* way);
	int countWays(unsigned int iWayId);
	DT_OSMWayBBox* getWay(unsigned int iWayId, unsigned int WayNo);
	DT_OSMWayBBox* getFirstWay();
	DT_OSMWayBBox* getNextWay();

};

#endif /* WAYSLIST_H_ */
