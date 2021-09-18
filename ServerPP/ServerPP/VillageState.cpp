#include "IOCPNet_RootHeader.h"

void VillageState::OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{
}

void VillageState::OnSendCompleted()
{
}

void VillageState::OnInitilzed()
{
	m_pVillageInfo = Village::FirstVillageInfo::Create<Village::FirstVillageInfo>();

}

void VillageState::OnChangedToThis(NetBase::OutputMemoryStreamPtr&)
{
#ifdef __DEBUG
#endif

	// 1. village info id 보내기
	// village 는 고정된 정보이니 id 만으로 client 에서 확인 가능할 것으로 보임
	auto owner = m_ownerPtr.lock();
	
}
