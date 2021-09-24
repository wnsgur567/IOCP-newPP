#include "IOCPNet_RootHeader.h"

void VillageState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream)
{
#ifdef __DEBUG
	printf("---VillageState OnRecvCompleted Process...\n");
#endif // __DEBUG

	ProtocolSize_t raw_protocol;
	NetBase::ReadFromBinStream(inpStream, raw_protocol);
	//inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);
	ESectorProtocol sector_protocol;
	GetProtocol(raw_protocol, sector_protocol);

	switch (protocol)
	{
	case EProtocol::EnterVillage:
	{
		auto owner = m_ownerPtr.lock();
		Village::VillageManager::sInstance->VillageEnterProcess(inpStream, owner);
	}
	break;
	case EProtocol::ExitVillage:
	{
		auto owner = m_ownerPtr.lock();
		Village::VillageManager::sInstance->VillageExitProcess(inpStream, owner);
	}
	break;
	}

	switch (sector_protocol)
	{
	case ESectorProtocol::PlayerMoveAndAction:
	{
		auto owner = m_ownerPtr.lock();
		Village::VillageManager::sInstance->VillageExitProcess(inpStream, owner);
	}
	break;
	case ESectorProtocol::PlayerAction:
	{
		auto owner = m_ownerPtr.lock();
		Village::VillageManager::sInstance->VillageActionProcess(inpStream, owner);
	}
	break;
	case ESectorProtocol::PlayerMove:
	{
		auto owner = m_ownerPtr.lock();
		Village::VillageManager::sInstance->VillageMoveProcess(inpStream, owner);
	}
	break;
	}
}

void VillageState::OnSendCompleted()
{
}

void VillageState::OnInitilzed()
{

}

void VillageState::OnChangedToThis()
{
#ifdef  __DEBUG
	printf("start Village State!!\n");
#endif //  __DEBUG

	// 추후 저장된 user 정보에서 village id 가져 올 것		
	auto owner = m_ownerPtr.lock();
	m_current_result = Village::VillageManager::sInstance->StateChangedProcess(owner);
}

void VillageState::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{

	if ((ProtocolSize_t)EProtocol::EnterVillage & inOrigin)
	{
		outProtocol = EProtocol::EnterVillage;
		return;
	}
	if ((ProtocolSize_t)EProtocol::ExitVillage & inOrigin)
	{
		outProtocol = EProtocol::EnterVillage;
		return;
	}

	
}

void VillageState::GetProtocol(ProtocolSize_t inOrigin, ESectorProtocol& outProtocol)
{
	if ((ProtocolSize_t)ESectorProtocol::PlayerMove & inOrigin)
	{
		if ((ProtocolSize_t)ESectorProtocol::PlayerAction & inOrigin)
		{
			outProtocol = ESectorProtocol::PlayerMoveAndAction;
			return;
		}
		outProtocol = ESectorProtocol::PlayerMove;
		return;
	}
	if ((ProtocolSize_t)ESectorProtocol::PlayerAction & inOrigin)
	{
		outProtocol = ESectorProtocol::PlayerAction;
		return;
	}


	
}

void VillageState::HandleAction(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{

}

void VillageState::HandleMove(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{


}

IOCPSession::ClientStatePtr VillageState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<VillageState> retState;
	retState.reset(new VillageState(pSession));

	return retState;
}
