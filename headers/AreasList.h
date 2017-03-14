#ifndef AREASLIST_H_
#define AREASLIST_H_

#include <map>
/* this file need more comments*/
extern "C" {
#include "DataStructures.h"
}
//true
using namespace std;

class CAreasList {
private:
	multimap<int, DT_OSMAreaBBox*> m_vAreas;

	//the map iterator
	multimap<int, DT_OSMAreaBBox*>::iterator m_MapIterator;
public:
	~CAreasList();
<<<<<<< HEAD

	int gyyAreasNo();
	int addArea(DT_OSMAreaBBox* area);
=======
	/*comment*/
	int getAreasNoo();
	
>>>>>>> origin/MihaelaVarga
	int countAreas(unsigned int iAreaId);
	DT_OSMAreaBBox* getArea(unsigned int iAreaId, unsigned int AreaNo);
	DT_OSMAreaBBox* getFirstArea();
	DT_OSMAreaBBox* getNextArea();
//false
};

#endif /* AREASLIST_H_ */
