#include "IOCPNet_RootHeader.h"

namespace Village
{
	int VillageInfoBase::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
	{
		int size = 0;
		WriteToStream(out_stream, m_village_id);
		return size;
	}
	int VillageInfoBase::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
	{
		int size = 0;
		ReadFromStream(in_stream, m_village_id);
		return size;
	}
}