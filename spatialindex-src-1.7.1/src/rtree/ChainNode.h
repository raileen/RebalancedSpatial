#pragma once
#include <Node.h>
namespace SpatialIndex
{
	namespace RTree
	{
		class ChainNode : public Node
		{
		public:
			ChainNode(void);
			~ChainNode(void);
		private:
			virtual void insertEntry(uint32_t dataLength, byte* pData, Region& mbr, id_type id);
			//virtual bool insertData(uint32_t dataLength, byte* pData, Region& mbr, id_type id, std::stack<id_type>& pathBuffer, byte* overflowTable);
			ChainNode* Next;
		};
	}
}
