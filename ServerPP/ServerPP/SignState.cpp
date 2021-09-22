#include "IOCPNet_RootHeader.h"


#pragma warning (disable : 6054)

void SignState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream)
{
#ifdef  __DEBUG
	printf("---SignState OnRecvCompleted Process...\n");
#endif //  __DEBUG

	ProtocolSize_t raw_protocol;
	NetBase::ReadFromBinStream(inpStream, raw_protocol);
	//inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);
	switch (protocol)
	{
	case SignState::EProtocol::SignIn:
	{
		// request sign in process
		auto owner = m_ownerPtr.lock();
		m_current_result = Sign::SignManager::sInstance->SignInProcess(inpStream, owner);		
	}
	break;

	case SignState::EProtocol::SignOut:
	{
		// request sign out process
		auto owner = m_ownerPtr.lock();
		m_current_result = Sign::SignManager::sInstance->SignOutProcess(inpStream, owner);
	}
	break;

	case SignState::EProtocol::SignUp:
	{
		// request sign up process	
		auto owner = m_ownerPtr.lock();
		m_current_result = Sign::SignManager::sInstance->SignUpProcess(inpStream, owner);
	}
	break;

	case SignState::EProtocol::DeleteAccount:
	{
		// request sign up process	
		auto owner = m_ownerPtr.lock();
		m_current_result = Sign::SignManager::sInstance->DeleteAccountProcess(inpStream, owner);
	}
	break;

	default:
	{
		throw Utils::NotImplementException();
	}
	break;
	}
}

void SignState::OnSendCompleted()
{
	// change state
	switch (m_current_result)
	{
	case EResult::Success_SingIn:
	{
		auto owner = m_ownerPtr.lock();		
		owner->m_isSigned = true;
		owner->ChangeState(owner->m_characterSelect_state);
#ifdef __DEBUG
		printf("change client state : sign -> character select\n");
#endif
	}
	break;
	case EResult::Success_SignOut:
	case EResult::Success_DeleteAccount:
	{
		auto owner = m_ownerPtr.lock();
		owner->ChangeState(owner->m_characterSelect_state);
#ifdef __DEBUG
		printf("change client state : sign -> character select\n");
#endif
	}
	break;
	}
}

void SignState::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{
	if ((ProtocolSize_t)EProtocol::SignIn & inOrigin)
	{
		outProtocol = EProtocol::SignIn;
		return;
	}
	if ((ProtocolSize_t)EProtocol::SignOut & inOrigin)
	{
		outProtocol = EProtocol::SignOut;
		return;
	}
	if ((ProtocolSize_t)EProtocol::SignUp & inOrigin)
	{
		outProtocol = EProtocol::SignUp;
		return;
	}
	if ((ProtocolSize_t)EProtocol::DeleteAccount & inOrigin)
	{
		outProtocol = EProtocol::DeleteAccount;
		return;
	}

	throw Utils::NotImplementException();
}


IOCPSession::ClientStatePtr SignState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<SignState> retState;
	retState.reset(new SignState(pSession));

	return retState;
}

void SignState::OnInitilzed()
{
}

void SignState::OnChangedToThis()
{
	// 할 거 없음
}
