/*
 * MapInfo.h
 *
 *  Created on: Apr 22, 2009
 *      Author: vlad
 */

#ifndef MAPINFO_H_
#define MAPINFO_H_

#include "DataStructures.h"

class CMapInfo
{
private:
	DT_MapInfo *m_pMapInfo;

	static CMapInfo *s_instance;
	CMapInfo();
public:
	~CMapInfo();
	static CMapInfo *instance();
	DT_MapInfo *getMapInfo();
};

#endif /* MAPINFO_H_ */
