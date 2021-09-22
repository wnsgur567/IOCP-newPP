#include "IOCPNet_RootHeader.h"

void CharacterSelectState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream)
{
#ifdef __DEBUG
	printf("---CharacterSelectState OnRecvCompleted Process...\n");
#endif // __DEBUG

	ProtocolSize_t raw_protocol;
	NetBase::ReadFromBinStream(inpStream, raw_protocol);
	//inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);

	switch (protocol)
	{
	case EProtocol::CharacterSelect:
	{
		// confirm info
		auto owner = m_ownerPtr.lock();
		m_current_result = CharacterSelect::CharacterSelectManager::sInstance->CharacterSelectProcess(inpStream, owner);
	}
	break;
	case EProtocol::SignOut:
	{
		// TODO : ...
		//auto owner = m_ownerPtr.lock();
		//m_current_result = CharacterSelect::CharacterSelectManager::sInstance->CharacterSelectProcess(inpStream, owner);
	}
	break;
	default:
		throw Utils::NotImplementException();
		break;
	}

}

void CharacterSelectState::OnSendCompleted()
{
	switch (m_current_result)
	{
	case EResult::Success_CharacterSelect:
	{
		auto owner = m_ownerPtr.lock();
		owner->ChangeState(owner->m_village_state);
	}
	break;
	default:
		break;
	}
}

void CharacterSelectState::OnInitilzed()
{
}

void CharacterSelectState::OnChangedToThis()
{
#ifdef  __DEBUG
	printf("start Character Select State!!\n");
#endif //  __DEBUG	

	auto owner = m_ownerPtr.lock();
	m_current_result = CharacterSelect::CharacterSelectManager::sInstance->StateChangedProcess(owner);	
}


void CharacterSelectState::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{
	if ((ProtocolSize_t)EProtocol::CharacterSelect & inOrigin)
	{
		outProtocol = EProtocol::CharacterSelect;
		return;
	}
	if ((ProtocolSize_t)EProtocol::SignOut & inOrigin)
	{
		outProtocol = EProtocol::SignOut;
		return;
	}

	throw Utils::NotImplementException();
}


IOCPSession::ClientStatePtr CharacterSelectState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<CharacterSelectState> retState;
	retState.reset(new CharacterSelectState(pSession));

	return retState;
}