#include "WaysServer.h"
#include "DataSerializers.h"
#include "MapSettings.h"
#include <QtDebug>



#define debug(x) printf("%s\n", (x));

//#define _WAYS_SERVER_DEBUG

/**
 *	Constructor for WaysServer, specify default values
 **/
CWaysServer::CWaysServer() :
	m_iHorBBoxes(0),
	m_iVerBBoxes(0),
	m_infoBBoxOffsetTable(NULL),
	m_iaBBoxRepartition(NULL),
	m_infoBBoxRepartition(NULL),
	m_llaBBoxOffsets(NULL),
	m_iTotalBBoxes(0),
	m_iCurZoomLevel(0),
	m_iLastError(0),
	m_fDataFile(NULL),
	m_fInfoFile(NULL)
{

}

/**
 *	Destructor for WaysServer, delete data, close file
 **/
CWaysServer::~CWaysServer()
{
	//cleanup all data
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
	}

	if (m_fInfoFile != NULL) {
		fclose(m_fInfoFile);
	}

	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
	}

	if (m_llaBBoxOffsets != NULL) {
		delete []m_llaBBoxOffsets;
	}

	if (m_infoBBoxOffsetTable != NULL) {
		delete []m_infoBBoxOffsetTable;
	}

	if (m_infoBBoxRepartition != NULL) {
		delete []m_infoBBoxRepartition;
	}

	//cleanup the map
	map<int, CWaysList*>::iterator iter;
	for (iter = m_vBBoxWays.begin(); iter != m_vBBoxWays.end(); iter++) {
		//free each list
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_vBBoxWays.clear();

	map<int, map<int, DT_OSMWayInfo*>* >:: iterator it;
	for (it = m_vBBoxWaysInfo.begin(); it != m_vBBoxWaysInfo.end(); it++)
	{
		for (map<int, DT_OSMWayInfo*>::iterator it2 = it->second->begin(); it2 != it->second->end(); it2++)
			OSM_Free_WayInfo(it2->second);

		it->second->clear();
		delete it->second;
	}
	m_vBBoxWaysInfo.clear();
}

/**
 *	Get number of ways
 *	@return number of ways 
 **/
int
CWaysServer::getWaysNo()
{
	int res = 0;
	
	map<int, CWaysList*>::iterator iter;
	for (iter = m_vBBoxWays.begin(); iter != m_vBBoxWays.end(); iter++) {
		res += (*iter).second->getWaysNo();
	}
	
	return res;
}

/**
 *	Get last error
 *	@return last error variable value
 **/
int
CWaysServer::getLastError()
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
CWaysServer::loadBBoxData(int iBBoxNumber, int iZoomLevel)
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
        map<int, CWaysList*>::iterator iter = m_vBBoxWays.find((iBBoxNumber << 4) | tmpZoomLevel);
        if (iter == m_vBBoxWays.end())
        {
            //create new list and add it to map
            CWaysList *tmpList = new CWaysList();
            m_vBBoxWays[(iBBoxNumber << 4) | tmpZoomLevel] = tmpList;

            //search for ways in bbox
            iter = m_vBBoxWays.find((iBBoxNumber << 4) | tmpZoomLevel);

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
				DT_OSMWayBBox *tmpWay = NULL;
				DS_UnserializeOSMWayBBox(buf, &tmpWay);

				if ((*iter).second->addWay(tmpWay) < 0)
					OSM_Way_BBox_Free(tmpWay);

				delete[] buf;
			}
		}
		tmpZoomLevel++;
	}

    if (m_fInfoFile == NULL) {
    	m_iLastError = -1;
    	return -1;
    }

    //jump to the BBoxOffset
	fseek(m_fInfoFile, m_infoBBoxOffsetTable[iBBoxNumber], SEEK_SET);
	map<int, map<int, DT_OSMWayInfo*>* >::iterator it = m_vBBoxWaysInfo.find(iBBoxNumber);
	if (it == m_vBBoxWaysInfo.end())
	{
       	//create new list and add it to map
       	map<int, DT_OSMWayInfo*> *tmpInfoMap=new map<int, DT_OSMWayInfo*>();
       	m_vBBoxWaysInfo[iBBoxNumber]=tmpInfoMap;

       	for(unsigned int i=0;i<m_infoBBoxRepartition[iBBoxNumber];i++)
       	{
       		//read a new entry from the file
       		unsigned int bufferSize = 0;
       		if (fread(&bufferSize, sizeof(bufferSize), 1, m_fInfoFile) != 1) {
       			//some error while reading file
       			m_iLastError = -2;
       			return -2;
       		}
       		int bf=0;
       		fread(&bf,sizeof(int),1,m_fInfoFile);

       		//now read the buffer containing effective data
       		char *buf = new char[bufferSize];

       		if (!fread(buf, bufferSize, 1, m_fInfoFile) ){//!= bufferSize) {
       			m_iLastError = -2;
       			debug("X: error while reading file")
       			return -2;
       		}

       		//all data was read, unserialize the data
       		DT_OSMWayInfo *tmpInfo;
       		DS_UnserializeOSMWayInfo(bf, buf, &tmpInfo);

       		(*tmpInfoMap)[tmpInfo->id]=tmpInfo;

       		delete []buf;
       	}
	}

	m_iLastError = 0;

	return 0;
}

/**
 *	Delete bbox data with given number
 *	@param	iBBoxNumber - number of bbox to delete
 **/
void
CWaysServer::releaseBBoxData(int iBBoxNumber)
{
	int i =0;

	for (;i<NO_ZOOMLEVELS;i++)
	{
		map<int, CWaysList*>::iterator iter = m_vBBoxWays.find((iBBoxNumber << 4) | i);
		if ( iter == m_vBBoxWays.end() ) {
			//this BBox was not loaded yet. set last error and exit
			m_iLastError = -1;
			continue;
		}
		//delete the data
		delete (*iter).second;

		//erase iterator from list
		m_vBBoxWays.erase(iter);
	}

	map<int, map<int, DT_OSMWayInfo*>* >:: iterator it = m_vBBoxWaysInfo.find(iBBoxNumber);
	if(it != m_vBBoxWaysInfo.end())
	{
		for (map<int, DT_OSMWayInfo*>::iterator it2 = it->second->begin(); it2 != it->second->end(); it2++)
			OSM_Free_WayInfo(it2->second);

		it->second->clear();
		delete it->second;

		m_vBBoxWaysInfo.erase(it);
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
CWaysServer::countWays(int iWayId, int iBBoxNumber, int iZoomLevel)
{
	map<int, CWaysList*>::iterator iter = m_vBBoxWays.find((iBBoxNumber << 4) | iZoomLevel);

	if ( iter != m_vBBoxWays.end() )
		return (*iter).second->countWays(iWayId);

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
DT_OSMWayBBox*
CWaysServer::getWay(int iWayId, int iBBoxNumber, int iZoomLevel, int iWayNo/* =0 */)
{
	DT_OSMWayBBox* way = NULL;

	map<int, CWaysList*>::iterator iter = m_vBBoxWays.find((iBBoxNumber << 4) | iZoomLevel);
	way = (*iter).second->getWay(iWayId, iWayNo);

	return way;
}

/**
 *	Get first way from given bbox
 *	@param	iBBoxNumber - number of bbox
 *	@return pointer to the way found, NULL otherwise
 **/
DT_OSMWayBBox*
CWaysServer::getFirstWay(int iBBoxNumber, int iZoomLevel, E_ServerType type /* = E_ZoomSpecific*/)
{
	int i;
	DT_OSMWayBBox* way = NULL;
	map<int, CWaysList*>::iterator iter;

#ifdef _WAYS_SERVER_DEBUG
		printf("---------CWaysServer::getFirstWay(%d, %d)------------\n", iBBoxNumber, iZoomLevel);
#endif

	if (type == E_ZoomSpecific) {
		iter = m_vBBoxWays.find((iBBoxNumber << 4) | iZoomLevel);
		if (iter != m_vBBoxWays.end()) {
			way = ((*iter).second->getFirstWay());
			m_iCurZoomLevel = iZoomLevel;
		}
	}
	else //if (type == E_MaximumZoom)
	{
		// Move all internal iterators from CWaysList to the first element and return the first available way
		for (i = iZoomLevel; i >= 0; i--)
		{
			iter = m_vBBoxWays.find((iBBoxNumber << 4) | i);
			if (iter != m_vBBoxWays.end()) {
				way = ((*iter).second->getFirstWay());
				if (way!= NULL)
					m_iCurZoomLevel = i;
#ifdef _WAYS_SERVER_DEBUG
			else
				printf("way is NULL \n");
#endif
			}
		}
	}
	return way;
}

/**
 *	Get next way from given bbox
 *	@param	iBBoxNumber - number of bbox
 *	@return	pointer to the way found, NULL otherwise
 **/
DT_OSMWayBBox*
CWaysServer::getNextWay(int iBBoxNumber, int iZoomLevel, E_ServerType type /* = E_ZoomSpecific*/)
{
	int i;
	DT_OSMWayBBox* way = NULL;
	map<int, CWaysList*>::iterator iter;

	if (type == E_ZoomSpecific) {
		iter = m_vBBoxWays.find((iBBoxNumber << 4) | iZoomLevel);
		if (iter != m_vBBoxWays.end()) {
			way = ((*iter).second->getNextWay());
		}
	}
	else //if (type == E_MaximumZoom)
	{
		iter = m_vBBoxWays.find((iBBoxNumber << 4) | m_iCurZoomLevel);
		if (iter != m_vBBoxWays.end()) {
			way = ((*iter).second->getNextWay());
		}
		if (way == NULL) {
			m_iCurZoomLevel++;
			for (i = m_iCurZoomLevel; i <= iZoomLevel; i++)
			{
				map<int, CWaysList*>::iterator iter = m_vBBoxWays.find((iBBoxNumber << 4) | i);
				if (iter != m_vBBoxWays.end()) {
					way = ((*iter).second->getFirstWay());
					if (way!= NULL) {
						m_iCurZoomLevel = i;
						break;
					}
				}
			}
		}
	}
	return way;
}

/**
 *  Get way info for a particular way
 *  @param iBBoxNumber - number of BBox
 *  @param iWayId - number of way
 *  @return pointer to DT_OSMWayInfo structure with way's info
 **/

DT_OSMWayInfo*
CWaysServer::getWayInfo(int iBBoxNumber,int iWayId)
{
	map<int, map<int, DT_OSMWayInfo*>*>::iterator iter = m_vBBoxWaysInfo.find(iBBoxNumber);
    if (iter == m_vBBoxWaysInfo.end()){
        return NULL;
    }
    map<int, DT_OSMWayInfo*>::iterator iter2=iter->second->find(iWayId);
    if (iter2 == iter->second->end()){
        return NULL;
    }

    return iter2->second;
}


/**
 *	Open specified file, read data, create structures
 *	@param	fileName - name of the file to open
 *	@return 0 for success, -1 for error
 **/
int
CWaysServer::openFile(const char *fileName,const char *fileName2)
{
	if (m_fDataFile != NULL) {
		fclose(m_fDataFile);
		m_fDataFile = NULL;
	}

	if (m_fInfoFile != NULL) {
		fclose(m_fInfoFile);
		m_fInfoFile = NULL;
	}
	//open file
	m_fDataFile = fopen(fileName, "rb");
	if (m_fDataFile == NULL) {
		printf("Error while opening file\n");
		m_iLastError = -1;
		return -1;
	}

	m_fInfoFile = fopen(fileName2, "rb");
	if (m_fInfoFile == NULL) {
		printf("Error while opening file\n");
		m_iLastError = -1;
		return -1;
	}

	//start reading file information
	fread(&m_iHorBBoxes, sizeof(m_iHorBBoxes), 1, m_fDataFile);
	fread(&m_iVerBBoxes, sizeof(m_iVerBBoxes), 1, m_fDataFile);
	fseek(m_fInfoFile, sizeof(int)*2, SEEK_SET);

	m_iTotalBBoxes = m_iHorBBoxes * m_iVerBBoxes;

	//delete any previous structures
	if (m_iaBBoxRepartition != NULL) {
		delete []m_iaBBoxRepartition;
		delete []m_llaBBoxOffsets;
	}

	if (m_infoBBoxOffsetTable != NULL) {
		delete []m_infoBBoxRepartition;
		delete []m_infoBBoxOffsetTable;
	}

    //create structures
	m_iaBBoxRepartition = new unsigned int[m_iTotalBBoxes];
	m_infoBBoxRepartition = new unsigned int[m_iTotalBBoxes];
	m_llaBBoxOffsets = new unsigned long long[m_iTotalBBoxes];
	m_infoBBoxOffsetTable = new long long[m_iTotalBBoxes];

	//read structures from file
	fread(m_infoBBoxRepartition,sizeof(unsigned int),m_iTotalBBoxes,m_fInfoFile);
	fread(m_infoBBoxOffsetTable, sizeof(long long)*m_iTotalBBoxes,1,m_fInfoFile);
	fread(m_iaBBoxRepartition, sizeof(unsigned int), m_iTotalBBoxes, m_fDataFile);
	fread(m_llaBBoxOffsets, sizeof(long long), m_iTotalBBoxes, m_fDataFile);

	m_iLastError = 0;

	return 0;
}
