#include "IOCPNet_RootHeader.h"

void VillageState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
#ifdef __DEBUG
	printf("---VillageState OnRecvCompleted Process...\n");
#endif // __DEBUG

	ProtocolSize_t raw_protocol;
	NetBase::ReadFromBinStream(inpStream, raw_protocol);
	//inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);

	switch (protocol)
	{
	case EProtocol::EnterVillage:
		// 마을 확장 시
		break;
	case EProtocol::ExitVillage:
		// 마을 확장 시
		break;

	case EProtocol::PlayerAction:
		break;
	case EProtocol::PlayerMove:
		break;
	case EProtocol::PlayerMoveAndAction:
		break;

	default:
		throw Utils::NotImplementException();
		break;
	}
}

void VillageState::OnSendCompleted()
{
}

void VillageState::OnInitilzed()
{
}

void VillageState::OnChangedToThis(NetBase::OutputMemoryStreamPtr& outpStream)
{
#ifdef  __DEBUG
	printf("start Village State!!\n");
#endif //  __DEBUG

	auto resultData = Village::VillageManager::sInstance->VillageChangedProcess(m_pVillageInfo->GetVillageID());
	m_current_result = resultData.result;
	outpStream = resultData.outpStream;
}

void VillageState::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{
	if ((ProtocolSize_t)EProtocol::PlayerAction & inOrigin)
	{
		if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
		{
			outProtocol = EProtocol::PlayerMoveAndAction;
			return;
		}
		outProtocol = EProtocol::PlayerAction;
		return;
	}
	if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
	{
		outProtocol = EProtocol::PlayerMove;
		return;
	}
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

	throw Utils::NotImplementException();
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
