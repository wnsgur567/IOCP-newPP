#include "IOCPNet_RootHeader.h"

Implementation_sInstance(NetGameObjectManager);

bool NetGameObjectManager::Initialize(LPVOID) noexcept
{
	m_newObj_id = 1;
	return true;
}
void NetGameObjectManager::Finalize() noexcept
{

}

NetGameObjectPtr NetGameObjectManager::GetNetObject(uint64_t inNetobj_id)
{
	if (m_netobject_map.end() == m_netobject_map.find(inNetobj_id))
		return nullptr;
	return m_netobject_map[inNetobj_id];
}

void NetGameObjectManager::Destroy(uint64_t inNetobj_id)
{
	if (m_netobject_map.end() != m_netobject_map.find(inNetobj_id))
	{
		m_netobject_map[inNetobj_id]->BeforeDestroy();
		m_netobject_map.erase(inNetobj_id);
	}
}
