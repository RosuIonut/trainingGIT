#ifndef AREASLIST_H_
#define AREASLIST_H_

#include <map>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CAreasList {
private:
	multimap<int, DT_OSMAreaBBox*> m_vAreas;

	//the map iterator
	multimap<int, DT_OSMAreaBBox*>::iterator m_MapIterator;
public:
	CAreasList();
	~CAreasList();

	int getAreasNo();
	int addArea(DT_OSMAreaBBox* area);
	int countAreas(unsigned int iAreaId);
	DT_OSMAreaBBox* getArea(unsigned int iAreaId, unsigned int AreaNo);
	DT_OSMAreaBBox* getFirstArea();
	DT_OSMAreaBBox* getNextArea();

};

#endif /* AREASLIST_H_ */
