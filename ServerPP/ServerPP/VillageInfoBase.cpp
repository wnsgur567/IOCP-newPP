#include "IOCPNet_RootHeader.h"

namespace Village
{
	// �ش� player�� ���� village �� ���Ϳ� ��ġ��Ŵ
	void VillageInfoBase::RegistEnterPlayerObj(PlayerInfoPtr inpPlayerInfo)
	{

		m_IdToPlayer_map.insert({ inpPlayerInfo->GetNetID(),  inpPlayerInfo });

		// callback
		OnPlayerRegisted(inpPlayerInfo);
	}
	void VillageInfoBase::DeleteExitPlayerObj(uint32_t in_netobj_id)
	{
		// callback
		BeforePlayerDelete(m_IdToPlayer_map[in_netobj_id]);

		m_IdToPlayer_map.erase(in_netobj_id);
	}
	int VillageInfoBase::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
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