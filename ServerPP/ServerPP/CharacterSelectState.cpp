#include "IOCPNet_RootHeader.h"

void CharacterSelectState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{


}

void CharacterSelectState::OnSendCompleted()
{
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

	auto owner = m_ownerPtr.lock();	// session ptr	
	auto resultData = CharacterSelect::CharacterSelectManager::sInstance->StateInitializeProcess(owner->m_user_id);
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

void CharacterSelectState::HandleCharacterSelectPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{
}

void CharacterSelectState::HandleSignOutPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{
}

IOCPSession::ClientStatePtr CharacterSelectState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<CharacterSelectState> retState;
	retState.reset(new CharacterSelectState(pSession));

	return retState;
}


