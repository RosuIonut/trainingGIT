#include "AreasServer.h"
#include "DataSerializers.h"
#include "MapSettings.h"

#define debug(x) printf("%s\n", (x));

//#define _WAYS_SERVER_DEBUG

/**
 *	Constructor for WaysServer, specify default values
 **/
CAreasServer::CAreasServer() :
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_iaBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0),
	m_iCurZoomLevel(0),
	m_iLastError(0),
	m_fDataFile(NULL)
{

}

/**
 *	Destructor for WaysServer, delete data, close file
 **/
CAreasServer::~CAreasServer()
{
	//cleanup all data
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
	}

	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
	}

	if (m_llaBBoxOffsets != NULL) {
		delete []m_llaBBoxOffsets;
	}

	//cleanup the map
	map<int, CAreasList*>::iterator iter;
	for (iter = m_vBBoxAreas.begin(); iter != m_vBBoxAreas.end(); iter++) {
		//free each list
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_vBBoxAreas.clear();
}

/**
 *	Get number of ways
 *	@return number of ways 
 **/
int
CAreasServer::getAreasNo()
{
	int res = 0;
	
	map<int, CAreasList*>::iterator iter;
	for (iter = m_vBBoxAreas.begin(); iter != m_vBBoxAreas.end(); iter++) {
		res += (*iter).second->getAreasNo();
	}
	
	return res;
}

/**
 *	Get last error
 *	@return last error variable value
 **/
int
CAreasServer::getLastError()
{
	return m_iLastError;
}

/**
 *	Load data of a bbox with given number and zoom level
 *	@param	iBBoxNumber - number of bbox
 *	@param	iZoomLevel - zoom level
 *	@return 0 for success, -1 for error
 **/
int
CAreasServer::loadBBoxData(int iBBoxNumber, int iZoomLevel)
{
	int tmpZoomLevel = 0;
	long long zoom_level_offset[NO_ZOOMLEVELS];
	unsigned int zoom_level_no_ways[NO_ZOOMLEVELS];

	if (m_fDataFile == NULL) {
		m_iLastError = -1;
		return -1;
	}


	//jump to the BBoxOffset
	fseek(m_fDataFile, m_llaBBoxOffsets[iBBoxNumber], SEEK_SET);

	fread(zoom_level_offset, sizeof(long long), NO_ZOOMLEVELS, m_fDataFile);
	fread(zoom_level_no_ways, sizeof(unsigned int), NO_ZOOMLEVELS, m_fDataFile);

	while (tmpZoomLevel <= iZoomLevel)
	{
		//iterate through ways
		map<int, CAreasList*>::iterator iter = m_vBBoxAreas.find((iBBoxNumber << 4) | tmpZoomLevel);

		if (iter == m_vBBoxAreas.end())
		{
			//create new list and add it to map
			CAreasList *tmpList = new CAreasList();
			m_vBBoxAreas[(iBBoxNumber << 4) | tmpZoomLevel] = tmpList;

			//search for ways in bbox
			iter = m_vBBoxAreas.find((iBBoxNumber << 4) | tmpZoomLevel);
		
			//jump to the zoom level offset
			fseek(m_fDataFile, zoom_level_offset[tmpZoomLevel], SEEK_SET);

			while (zoom_level_no_ways[tmpZoomLevel] > 0)
			{
				zoom_level_no_ways[tmpZoomLevel]--;

				//read a new entry from the file
				unsigned int bufferSize = 0;
				if (fread(&bufferSize, sizeof(bufferSize), 1, m_fDataFile) != 1) {
					//some error while reading file
					m_iLastError = -2;
					return -2;
				}

				//now read the buffer containing effective data
				char *buf = new char[bufferSize];
				if (fread(buf, sizeof(char), bufferSize, m_fDataFile) != bufferSize) {
					m_iLastError = -2;
					debug("error while reading file")
					return -2;
				}

				//all data was read, unserialize the data
				DT_OSMAreaBBox *tmpArea = NULL;
				DS_UnserializeOSMAreaBBox(buf, &tmpArea);

				if ((*iter).second->addArea(tmpArea) < 0)
					OSM_Area_BBox_Free(tmpArea);

				delete[] buf;
			}
		}
		tmpZoomLevel++;
	}

	m_iLastError = 0;

	return 0;
}

/**
 *	Delete bbox data with given number
 *	@param	iBBoxNumber - number of bbox to delete
 **/
void
CAreasServer::releaseBBoxData(int iBBoxNumber)
{
	int i =0;

	for (;i<NO_ZOOMLEVELS;i++)
	{
		map<int, CAreasList*>::iterator iter = m_vBBoxAreas.find((iBBoxNumber << 4) | i);
		if ( iter == m_vBBoxAreas.end() ) {
			//this BBox was not loaded yet. set last error and exit
			m_iLastError = -1;
			continue;
		}
		//delete the data
		delete (*iter).second;

		//erase iterator from list
		m_vBBoxAreas.erase(iter);
	}

	m_iLastError = 0;
}

/**
 *	Count the number of ways
 *	@param	iWayId - id of way
 *	@param	iBBoxNumber - number of bbox containing it
 *	@param	iZoomLevel - zoomlevel of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
int
CAreasServer::countAreas(int iAreaId, int iBBoxNumber, int iZoomLevel)
{
	map<int, CAreasList*>::iterator iter = m_vBBoxAreas.find((iBBoxNumber << 4) | iZoomLevel);

	if ( iter != m_vBBoxAreas.end() )
		return (*iter).second->countAreas(iAreaId);

	return 0;
}

/**
 *	Get a specified way
 *	@param	iWayId - id of way
 *	@param	iBBoxNumber - number of bbox containing it
 *	@param	iZoomLevel - zoomlevel of the way
 *	@param	iWayNo - number of way with the same id
 *	@return pointer to the way if found, NULL otherwise
 **/
DT_OSMAreaBBox*
CAreasServer::getArea(int iAreaId, int iBBoxNumber, int iZoomLevel, int iAreaNo/* =0 */)
{
	DT_OSMAreaBBox* area = NULL;

	map<int, CAreasList*>::iterator iter = m_vBBoxAreas.find((iBBoxNumber << 4) | iZoomLevel);
	area = (*iter).second->getArea(iAreaId, iAreaNo);

	return area;
}

/**
 *	Get first way from given bbox
 *	@param	iBBoxNumber - number of bbox
 *	@return pointer to the way found, NULL otherwise
 **/
DT_OSMAreaBBox*
CAreasServer::getFirstArea(int iBBoxNumber, int iZoomLevel, E_ServerType type /* = E_ZoomSpecific*/)
{
	int i;
	DT_OSMAreaBBox* area = NULL;
	map<int, CAreasList*>::iterator iter;

	if (type == E_ZoomSpecific) {
		iter = m_vBBoxAreas.find((iBBoxNumber << 4) | iZoomLevel);
		if (iter != m_vBBoxAreas.end()) {
			area = ((*iter).second->getFirstArea());
			m_iCurZoomLevel = iZoomLevel;
		}
	}
	else //if (type == E_MaximumZoom)
	{
		// Move all internal iterators from CWaysList to the first element and return the first available way
		for (i = iZoomLevel; i >= 0; i--)
		{
			iter = m_vBBoxAreas.find((iBBoxNumber << 4) | i);
			if (iter != m_vBBoxAreas.end()) {
				area = ((*iter).second->getFirstArea());
				if (area!= NULL)
					m_iCurZoomLevel = i;
			}
		}
	}
	return area;
}

/**
 *	Get next way from given bbox
 *	@param	iBBoxNumber - number of bbox
 *	@return	pointer to the way found, NULL otherwise
 **/
DT_OSMAreaBBox*
CAreasServer::getNextArea(int iBBoxNumber, int iZoomLevel, E_ServerType type /* = E_ZoomSpecific*/)
{
	int i;
	DT_OSMAreaBBox* area = NULL;
	map<int, CAreasList*>::iterator iter;

	if (type == E_ZoomSpecific) {
		iter = m_vBBoxAreas.find((iBBoxNumber << 4) | iZoomLevel);
		if (iter != m_vBBoxAreas.end()) {
			area = ((*iter).second->getNextArea());
		}
	}
	else //if (type == E_MaximumZoom)
	{
		iter = m_vBBoxAreas.find((iBBoxNumber << 4) | m_iCurZoomLevel);
		if (iter != m_vBBoxAreas.end()) {
			area = ((*iter).second->getNextArea());
		}
		if (area == NULL) {
			m_iCurZoomLevel++;
			for (i = m_iCurZoomLevel; i <= iZoomLevel; i++)
			{
				map<int, CAreasList*>::iterator iter = m_vBBoxAreas.find((iBBoxNumber << 4) | i);
				if (iter != m_vBBoxAreas.end()) {
					area = ((*iter).second->getFirstArea());
					if (area!= NULL) {
						m_iCurZoomLevel = i;
						break;
					}
				}
			}
		}
	}
	return area;
}

/**
 *	Open specified file, read data, create structures
 *	@param	fileName - name of the file to open
 *	@return 0 for success, -1 for error
 **/
int
CAreasServer::openFile(const char *fileName)
{
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
		m_fDataFile = NULL;
	}

	//open file
	m_fDataFile = fopen(fileName, "rb");
	if (m_fDataFile == NULL) {
		printf("Error while opening file\n");
		m_iLastError = -1;
		return -1;
	}

	//start reading file information
	fread(&m_iHorBBoxes, sizeof(m_iHorBBoxes), 1, m_fDataFile);
	fread(&m_iVerBBoxes, sizeof(m_iVerBBoxes), 1, m_fDataFile);

	m_iTotalBBoxes = m_iHorBBoxes * m_iVerBBoxes;

	//delete any previous structures
	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
		delete []m_llaBBoxOffsets;
	}

	//create structures
	m_iaBBoxRepartition = new unsigned int[m_iTotalBBoxes];
	m_llaBBoxOffsets = new unsigned long long[m_iTotalBBoxes];

	//read structures from file
	fread(m_iaBBoxRepartition, sizeof(unsigned int), m_iTotalBBoxes, m_fDataFile);
	fread(m_llaBBoxOffsets, sizeof(long long), m_iTotalBBoxes, m_fDataFile);

	m_iLastError = 0;

	return 0;
}




Test Commit asdjkhjdf!!
