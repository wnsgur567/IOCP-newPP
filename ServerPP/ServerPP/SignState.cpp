#include "IOCPNet_RootHeader.h"


#pragma warning (disable : 6054)

void SignState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
#ifdef  __DEBUG
	printf("---SignState OnRecvCompleted Process...\n");
#endif //  __DEBUG

	ProtocolSize_t raw_protocol;
	inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);
	switch (protocol)
	{
	case SignState::EProtocol::SignIn:
	{
		HandleSignInPacket(inpStream, outpStream);
	}
	break;

	case SignState::EProtocol::SignOut:
	{
		HandleSignOutPacket(inpStream, outpStream);
	}
	break;

	case SignState::EProtocol::SignUp:
	{
		HandleSignUpPacket(inpStream, outpStream);
	}
	break;

	case SignState::EProtocol::DeleteAccount:
	{
		HandleDeleteAccountPacket(inpStream, outpStream);
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

void SignState::HandleSignInPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
	/// Read from Input Stream
		// get data from recv stream
	Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
	pInfo->Flush();
	ISerializable* ptr = pInfo.get();
	// get id and pw from stream
	NetBase::ReadFromBinStream(inpStream, ptr);

	// request sign in process
	auto resultData = Sign::SignManager::sInstance->SignInProcess(pInfo);
	m_current_result = resultData.result;
	auto owner = m_ownerPtr.lock();
	owner->m_user_id = resultData.user_id;


#ifdef __DEBUG
	wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
	printf("%d\n", resultData.result);
	wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG

	// get new send stream
	outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	// write result to send stream (only data part)
	// sendpacket is composed with (size + id + data)

	int write_size = 0;
	/// Write to Output Stream
	// sign data = protocol + result + msg len + msg
	// write protocol	
	write_size += NetBase::WriteToBinStream(outpStream, (ProtocolSize_t)resultData.protocol);
	// write result
	write_size += NetBase::WriteToBinStream(outpStream, (ResultSize_t)resultData.result);
	// write result msg
	write_size += NetBase::WriteToBinStream(outpStream, std::wstring(resultData.msg));
	/// write end

#ifdef __DEBUG
	printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG		
}

void SignState::HandleSignOutPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
	/// Read from Input Stream
		// get data from recv stream
	Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
	pInfo->Flush();
	ISerializable* ptr = pInfo.get();
	// get id and pw from stream
	NetBase::ReadFromBinStream(inpStream, ptr);

	// request sign in process
	auto resultData = Sign::SignManager::sInstance->SignOutProcess(pInfo);
	m_current_result = resultData.result;

#ifdef __DEBUG
	wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
	printf("%d\n", resultData.result);
	wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG

	// get new send stream
	outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	// write result to send stream (only data part)
	// sendpacket is composed with (size + id + data)

	int write_size = 0;
	/// Write to Output Stream
	// sign data = protocol + result + msg len + msg
	// write protocol	
	write_size += NetBase::WriteToBinStream(outpStream, (ProtocolSize_t)resultData.protocol);
	// write result
	write_size += NetBase::WriteToBinStream(outpStream, (ResultSize_t)resultData.result);
	// write result msg
	write_size += NetBase::WriteToBinStream(outpStream, std::wstring(resultData.msg));
	/// write end

#ifdef __DEBUG
	printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	
}

void SignState::HandleSignUpPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
	/// Read from Input Stream
		// get data from recv stream
	Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
	pInfo->Flush();
	ISerializable* ptr = pInfo.get();
	// get id and pw from stream
	NetBase::ReadFromBinStream(inpStream, ptr);

	// request sign in process
	auto resultData = Sign::SignManager::sInstance->SignUpProcess(pInfo);
	m_current_result = resultData.result;

#ifdef __DEBUG
	wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
	printf("%d\n", resultData.result);
	wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


	// get new send stream
	outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	// write result to send stream (only data part)
	// sendpacket is composed with (size + id + data)

	int write_size = 0;
	/// Write to Output Stream
	// sign data = protocol + result + msg len + msg
	// write protocol	
	write_size += NetBase::WriteToBinStream(outpStream, (ProtocolSize_t)resultData.protocol);
	// write result
	write_size += NetBase::WriteToBinStream(outpStream, (ResultSize_t)resultData.result);
	// write result msg
	write_size += NetBase::WriteToBinStream(outpStream, std::wstring(resultData.msg));
	/// write end

#ifdef __DEBUG
	printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	
}

void SignState::HandleDeleteAccountPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{
	/// Read from Input Stream
		// get data from recv stream
	Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
	pInfo->Flush();
	ISerializable* ptr = pInfo.get();
	// get id and pw from stream
	NetBase::ReadFromBinStream(inpStream, ptr);

	// request sign in process
	auto resultData = Sign::SignManager::sInstance->SignInProcess(pInfo);
	m_current_result = resultData.result;

#ifdef __DEBUG
	wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
	printf("%d\n", resultData.result);
	wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


	// get new send stream
	outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	// write result to send stream (only data part)
	// sendpacket is composed with (size + id + data)

	int write_size = 0;
	/// Write to Output Stream
	// sign data = protocol + result + msg len + msg
	// write protocol	
	write_size += NetBase::WriteToBinStream(outpStream, (ProtocolSize_t)resultData.protocol);
	// write result
	write_size += NetBase::WriteToBinStream(outpStream, (ResultSize_t)resultData.result);
	// write result msg
	write_size += NetBase::WriteToBinStream(outpStream, std::wstring(resultData.msg));
	/// write end

#ifdef __DEBUG
	printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	
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

void SignState::OnChangedToThis(NetBase::OutputMemoryStreamPtr& outpStream)
{
	outpStream = nullptr;
}
