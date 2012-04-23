#include "ReConstructStruct.h"
#include <spatialindex/SpatialIndex.h>
#include "RTree.h"
#include "Node.h"
#include "Leaf.h"
using namespace SpatialIndex;
using namespace SpatialIndex::RTree;



ReConstructStruct::ReConstructStruct(): m_mbr()
{
	nodeID=-1;
	counter=0;
	hasChain=false;

}

ReConstructStruct::~ReConstructStruct()
{
}


void ReConstructStruct::updateLeafMBR(Leaf& L)
{

	uint64_t itemNumber = sizeof((L.getmptree())->table)/sizeof((L.getmptree())->table[0]);
	//找到table中相应id的条目
	uint64_t id;
	SpatialIndex::Region oldmbr;
	for(uint64_t i = 0; i<itemNumber; i++)
	{
		if((L.getmptree())->table[i].nodeID == L.getmidentifier())
		{
			((L.getmptree())->table[i]).hasChain = true;
			oldmbr = ((L.getmptree())->table[i]).m_mbr;
			id = i;
			break;
		}
	}

	uint32_t Dimension = oldmbr.m_dimension;
	double* High = oldmbr.m_pHigh;
	double* Low = oldmbr.m_pLow;

	Node* nodes = L.chain;
	while(nodes->m_chainNext != 0)//这里的next==0
	{
		SpatialIndex::Region tmpmbr = nodes->getmnodembr();
		for(uint32_t d = 0; d < Dimension; d++ )
		{
			if(High[d] < tmpmbr.m_pHigh[d])
			{
				High[d] = tmpmbr.m_pHigh[d];
			}
			if(Low[d] > tmpmbr.m_pLow[d])
			{
				High[d] = tmpmbr.m_pLow[d];
			}
		}

		nodes=nodes->m_chainNext;
	}

	SpatialIndex::Region* newmbr = new Region(Low, High, Dimension);
	
	//((L.getmptree())->table[L.getmidentifier()]).m_mbr = *newmbr; 
	((L.getmptree())->table[id]).m_mbr = *newmbr; 

}





void ReConstructStruct::updateInnerMBR(Leaf& L, std::stack<id_type>& pathBuffer)
{
	Node& N = L;//当前node
	//Node& P = L;//父节点node
	SpatialIndex::Region p_mbr;//父节点mbr，将与路径上子节点的比较
	SpatialIndex::Region t_mbr = N.getmnodembr();
	//当没到root
	while(!pathBuffer.empty())
	{
		uint64_t id;
		//得到当前节点的parent
		id_type p_id = pathBuffer.top();
		pathBuffer.pop();
		//找到这个parent对应在table中的条目
		int64_t itemNumber = sizeof((N.getmptree())->table)/sizeof((N.getmptree())->table[0]);
		for(uint64_t i = 0; i<itemNumber; i++)
		{
			if((N.getmptree())->table[i].nodeID == p_id)
			{
				((N.getmptree())->table[i]).hasChain = true;
				p_mbr = ((N.getmptree())->table[i]).m_mbr;//当前节点的父节点table中的原mbr，要与当前节点比较
				id = i;
				break;
			}
		}
		

		uint32_t Dimension = p_mbr.m_dimension;
		double* High = p_mbr.m_pHigh;
		double* Low = p_mbr.m_pLow;

			for(uint32_t d = 0; d < Dimension; d++ )
			{
				if(High[d] < t_mbr.m_pHigh[d])
				{
					High[d] = t_mbr.m_pHigh[d];
				}
				if(Low[d] > t_mbr.m_pLow[d])
				{
					High[d] = t_mbr.m_pLow[d];
				}
			}
			
		SpatialIndex::Region* newmbr = new Region(Low, High, Dimension);
		//更新p节点的mbr
		((L.getmptree())->table[id]).m_mbr = *newmbr; 
		
		t_mbr = *newmbr;

		//把当前节点赋值成P
		//N = P  不需要，因为有pathbuffer


	}


}




void ReConstructStruct::updatePath(Leaf& L, std::stack<id_type>& pathBuffer)
{
updateLeafMBR(L);
updateInnerMBR(L, pathBuffer);
}

