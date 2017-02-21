/*
 * MapInfo.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: vlad
 */

#include "MapInfo.h"

#include <stdlib.h>
#include <stdio.h>

//#define _MAP_DRAW_DEBUG_

/**MapInfo instance  */
CMapInfo* CMapInfo::s_instance = NULL;

/*data file name (map_data1)*/
const char* dataFileName = "data/MapData.bin";

/**
 *	Constructor for MapInfo: opens data file, allocates space
 **/
CMapInfo::CMapInfo() :
	m_pMapInfo(NULL)
{
	//read the data info from file
	FILE* dataFile = NULL;
	dataFile = fopen(dataFileName, "rb");
	if (dataFile == NULL) {
		return;
	}

	//allocate space for structure and read data to it
	m_pMapInfo = (DT_MapInfo*)malloc(sizeof(DT_MapInfo));
	fread(m_pMapInfo, sizeof(DT_MapInfo), 1, dataFile);

#ifdef _MAP_DRAW_DEBUG_
	printf("Map limits\n");
	printf("	left:	%3d.%06d\n", MAP_COORD2DEG(m_pMapInfo->left), MAP_COORD2DECDEG(m_pMapInfo->left));
	printf("	up:	%3d.%06d\n", MAP_COORD2DEG(m_pMapInfo->up), MAP_COORD2DECDEG(m_pMapInfo->up));
	printf("	right:	%3d.%06d\n", MAP_COORD2DEG(m_pMapInfo->right), MAP_COORD2DECDEG(m_pMapInfo->right));
	printf("	down:	%3d.%06d\n", MAP_COORD2DEG(m_pMapInfo->down), MAP_COORD2DECDEG(m_pMapInfo->down));
	printf("\nMap units conversion hor: %d  and ver %d", m_pMapInfo->horMapUnits, m_pMapInfo->verMapUnits);
	printf("\nHorizontal bboxes: %d\n", m_pMapInfo->horBBoxNo);
	printf("Vertical bboxes: %d\n", m_pMapInfo->verBBoxNo);
#endif

	//cleanup
	fclose(dataFile);
}

/**
 *	Destructor for MapInfo, free memory
 **/
CMapInfo::~CMapInfo()
{
	if (m_pMapInfo != NULL) {
		free(m_pMapInfo);
	}
}

/**
 *	Getter for m_pMapInfo var
 *	@return DT_MapInfo pointer to the returned map info
 **/
DT_MapInfo *
CMapInfo::getMapInfo()
{
	return m_pMapInfo;
}

/**
 *	Method ensuring single instance of class
 *	@return instance of MapInfo if there is none
 **/
CMapInfo *
CMapInfo::instance()
{
	if (s_instance == NULL) {
		s_instance = new CMapInfo();
	}

	return s_instance;
}
