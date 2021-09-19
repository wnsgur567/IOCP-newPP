#include "IOCPNet_RootHeader.h"

void CharacterSelectState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
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
		HandleCharacterSelectPacket(inpStream, outpStream);
		break;
	case EProtocol::SignOut:
		HandleSignOutPacket(inpStream, outpStream);
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

void CharacterSelectState::OnChangedToThis(NetBase::OutputMemoryStreamPtr& outpStream)
{
#ifdef  __DEBUG
	printf("start Character Select State!!\n");
#endif //  __DEBUG

	// 1. user id를 확인
	// 2. 해당 유저에 대한 모든 캐릭터 정보 불러오기 (SQL)
	// 3. 불러온 정보를 Session 수준에서 저장
	// 4. 모든 캐릭터 정보를 전송

	auto owner = m_ownerPtr.lock();
	auto resultData = CharacterSelect::CharacterSelectManager::sInstance->StateChangedProcess(owner->m_user_id);
	m_current_result = resultData.result;

	outpStream = resultData.outpStream;
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

void CharacterSelectState::HandleCharacterSelectPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
	// get selected character info from stream
	CharacterInfoPtr pInfo = std::make_shared<CharacterInfo>();
	//ISerializable* ptr = pInfo.get();
	NetBase::ReadFromBinStream(inpStream, pInfo);
	auto owner = m_ownerPtr.lock();
	owner->m_player->SetCharacterInfo(pInfo);

	// confirm info
	auto resultData = CharacterSelect::CharacterSelectManager::sInstance->CharacterSelectProcess(pInfo);
	m_current_result = resultData.result;
	outpStream = resultData.outpStream;
	switch (resultData.result)
	{
	case EResult::Success_CharacterSelect:
		break;
	case EResult::UndefinedCharacter:
		break;

	default:
		break;
	}

}

void CharacterSelectState::HandleSignOutPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
}

IOCPSession::ClientStatePtr CharacterSelectState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<CharacterSelectState> retState;
	retState.reset(new CharacterSelectState(pSession));

	return retState;
}


