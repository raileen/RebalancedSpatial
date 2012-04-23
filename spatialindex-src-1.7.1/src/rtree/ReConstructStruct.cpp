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
	//�ҵ�table����Ӧid����Ŀ
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
	while(nodes->m_chainNext != 0)//�����next==0
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
	Node& N = L;//��ǰnode
	//Node& P = L;//���ڵ�node
	SpatialIndex::Region p_mbr;//���ڵ�mbr������·�����ӽڵ�ıȽ�
	SpatialIndex::Region t_mbr = N.getmnodembr();
	//��û��root
	while(!pathBuffer.empty())
	{
		uint64_t id;
		//�õ���ǰ�ڵ��parent
		id_type p_id = pathBuffer.top();
		pathBuffer.pop();
		//�ҵ����parent��Ӧ��table�е���Ŀ
		int64_t itemNumber = sizeof((N.getmptree())->table)/sizeof((N.getmptree())->table[0]);
		for(uint64_t i = 0; i<itemNumber; i++)
		{
			if((N.getmptree())->table[i].nodeID == p_id)
			{
				((N.getmptree())->table[i]).hasChain = true;
				p_mbr = ((N.getmptree())->table[i]).m_mbr;//��ǰ�ڵ�ĸ��ڵ�table�е�ԭmbr��Ҫ�뵱ǰ�ڵ�Ƚ�
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
		//����p�ڵ��mbr
		((L.getmptree())->table[id]).m_mbr = *newmbr; 
		
		t_mbr = *newmbr;

		//�ѵ�ǰ�ڵ㸳ֵ��P
		//N = P  ����Ҫ����Ϊ��pathbuffer


	}


}




void ReConstructStruct::updatePath(Leaf& L, std::stack<id_type>& pathBuffer)
{
updateLeafMBR(L);
updateInnerMBR(L, pathBuffer);
}

