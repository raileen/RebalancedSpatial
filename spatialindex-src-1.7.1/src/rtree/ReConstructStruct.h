#pragma once
#include "tools/Tools.h"
#include <spatialindex/SpatialIndex.h>
#include <Region.h>

namespace SpatialIndex
{
	namespace RTree
	{
class ReConstructStruct
{

	friend class Leaf;
	friend class RTree;

public:
	int64_t nodeID;
	int64_t counter;
	bool hasChain;
	//MBR
	SpatialIndex::Region m_mbr;
	//uint32_t m_dimension;
	//double* m_pLow;
	//double* m_pHigh;


	ReConstructStruct();
	~ReConstructStruct();

	void updateLeafMBR(Leaf& L);	

	void updateInnerMBR(Leaf& L, std::stack<id_type>& pathBuffer);

	void updatePath(Leaf& L, std::stack<id_type>& pathBuffer);

};

	}
}