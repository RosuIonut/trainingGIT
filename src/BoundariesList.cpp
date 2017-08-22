ries.end(); iter++) {
		OSM_Boundary_BBox_Free((*iter).second);
	}

	m_vBoundaries.clear();
}

/**
 *	Get number of ways
 *	@return size of ways value
 **/
int
CBoundariesList::getBoundariesNo()
{
	return (m_vBoundaries.size());
}

/**
 *	Add a new way
 *	@param	way - DT_OSMWayBBox pointer to the way to be added
 **/
int
CBoundariesList::addBoundary(DT_OSMBoundaryBBox* boundary)
{
	//add the data to the list
	m_vBoundaries.insert(pair<int, DT_OSMBoundaryBBox*>((int)boundary->id, boundary));

	//reset the iterator for the list to the end of the list
	m_MapIterator = m_vBoundaries.end();

	return 0;
}

/**
 *	Counts no of ways with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
int
CBoundariesList::countBoundaries(unsigned int iBoundaryId)
{
	return m_vBoundaries.count(iBoundaryId);
}

/**
 *	Get way with given id
 *	@param	iWayId - id of the way
 *	@return pointer to the way if found, NULL otherwise
 **/
DT_OSMBoundaryBBox*
CBoundariesList::getBoundary(unsigned int iBoundaryId, unsigned int iBoundaryNo)
{
	multimap<int, DT_OSMBoundaryBBox*>::iterator iter;

	iter = m_vBoundaries.find(iBoundaryId);
	if (iter == m_vBoundaries.end()) {
		//the way id is not found at this zoom level
		return NULL;
	}
	while (iBoundaryNo--) {
		iter++;
		if (iter == m_vBoundaries.end())
			return NULL;
	}

	//we found it so we return it
	return ((*iter).second);
}

/**
 *	Get first way from ways list
 *	@return first way if exists, NULL otherwise
 **/
DT_OSMBoundaryBBox*
CBoundariesList::getFirstBoundary()
{
	m_MapIterator = m_vBoundaries.begin();

	if (m_MapIterator == m_vBoundaries.end()) {
		//the map is null now
		return NULL;
	}

	//we found something so we return it
	return ((*m_MapIterator).second);
}

/**
 *	Get next way from ways list
 *	@return pointer to the next way
 **/
DT_OSMBoundaryBBox*
CBoundariesList::getNextBoundary()
{
	if (m_MapIterator == m_vBoundaries.end()) {
		//we are at the end or the map is empty
		return NULL;
	}

	m_MapIterator++;

	if (m_MapIterator == m_vBoundaries.end()) {
		//we reached the end of the map
		return NULL;
	}

	//we found something so we return it
	return ((*m_MapIterator).second);
}



