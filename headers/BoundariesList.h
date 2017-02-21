#ifndef BOUNDARIESLIST_H_
#define BOUNDARIESLIST_H_

#include <map>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CBoundariesList {
private:
	multimap<int, DT_OSMBoundaryBBox*> m_vBoundaries;

	//the map iterator
	multimap<int, DT_OSMBoundaryBBox*>::iterator m_MapIterator;
public:
	CBoundariesList();
	~CBoundariesList();

	int getBoundariesNo();
	int addBoundary(DT_OSMBoundaryBBox* boundary);
	int countBoundaries(unsigned int iBoundaryId);
	DT_OSMBoundaryBBox* getBoundary(unsigned int iBoundaryId, unsigned int BoundaryNo);
	DT_OSMBoundaryBBox* getFirstBoundary();
	DT_OSMBoundaryBBox* getNextBoundary();

};

#endif /* BOUNDARIESLIST_H_ */
