/*
 * NodesServer.cpp
 *
 *  Created on: Apr 21, 2009
 *      Author: vlad
 */

#ifndef NODESSERVER_H_
#define NODESSERVER_H_

#include <stdio.h>
#include <map>

#include "DataStructures.h"
#include "NodesList.h"

using namespace std;

class CNodesServer
{

private:
	FILE*		m_fDataFile;
	int		m_iHorBBoxes;
	int		m_iVerBBoxes;
	unsigned int *m_iaBBoxRepartition;
	unsigned long long *m_llaBBoxOffsets;
	int 		m_iTotalBBoxes;

	map<int, CNodesList*> m_vBBoxNodes;
public:
	CNodesServer();
	~CNodesServer();

	int	loadBBoxData(int boxNumber);
	int	releaseBBoxData(int boxNumber);
	int	openFile(const char *fileName);

	int getNodesNo();
	DT_BBoxNode* getNode(int iNodeId, int iBBoxNumber);
	DT_BBoxNode* getFirstNode(int iBBoxNumber);
	DT_BBoxNode* getNextNode(int iBBoxNumber);
};

#endif /* NODESSERVER_H_ */
