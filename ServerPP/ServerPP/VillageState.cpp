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

	// 1. village info id ������
	// village �� ������ �����̴� id ������ client ���� Ȯ�� ������ ������ ����
	auto owner = m_ownerPtr.lock();
	
}
