/*
 * NodesList.h
 *
 *  Created on: Apr 30, 2009
 *      Author: Vlad
 */

#ifndef NODESLIST_H_
#define NODESLIST_H_

#include <vector>

extern "C" {
#include "DataStructures.h"
}

using namespace std;

class CNodesList {
private:
	vector<DT_BBoxNode*> m_vNodes;

	vector<DT_BBoxNode*>::iterator m_vIterator;
public:
	CNodesList();
	~CNodesList();

	int getNodesNo();
	void addNode(DT_BBoxNode *node);
	DT_BBoxNode* getNode(int nodeId);
	DT_BBoxNode* getFirstNode();
	DT_BBoxNode* getNextNode();
};

#endif /* NODESLIST_H_ */
