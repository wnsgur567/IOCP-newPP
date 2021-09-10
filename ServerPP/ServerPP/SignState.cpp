#include "IOCPNet_RootHeader.h"

#pragma warning (disable : 6054)
//#define TESTING

void SignState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{

#ifdef TESTING
	// ...

	printf("테스트 시작\n");

	char msg[512];

	int msg_length = 0;
	inpStream->Read(&msg_length, sizeof(msg_length));
	inpStream->Read(&msg, static_cast<size_t>(msg_length));
	msg[msg_length + 1] = NULL;


	printf("\n전송된 메시지 길이 : %d\n", msg_length);
	printf("전송된 메세지 : %s", msg);


	auto pSendStream = PacketManager::sInstance->GetSendStreamFromPool();

	pSendStream->Write(&msg_length, sizeof(msg_length));
	pSendStream->Write(msg, msg_length);

	printf("\n그대로 다시 전송...\n");

	return pSendStream;
#endif

#ifdef  __DEBUG
	printf("---SignState OnRecvCompleted Process...\n");
#endif //  __DEBUG

	Sign::SignManager::EResult retResult = Sign::SignManager::EResult::None;

	EProtocol protocol;

	inpStream->Read(&protocol, sizeof(EProtocol));

	switch (protocol)
	{
	case SignState::EProtocol::SignIn:
	{
		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfo info;
		info.Flush();

		// get id from stream
		wchar_t id_from_stream[Sign::SignInfo::MAX_IDSIZE];
		int id_length = 0;	// byte length...
		inpStream->Read(&id_length, sizeof(int));
		inpStream->Read(
			id_from_stream,
			id_length);
		// after last word must be \0
		// unicode is 2bytes per word
		id_from_stream[id_length / 2] = L'\0'; 
		info.ID = id_from_stream;
		// get pw from stream
		wchar_t pw_from_stream[Sign::SignInfo::MAX_PWSIZE];
		int pw_length = 0;
		inpStream->Read(&pw_length, sizeof(int));
		inpStream->Read(
			pw_from_stream,
			pw_length);
		// after last word must be \0
		// unicode is 2bytes per word
		pw_from_stream[pw_length / 2] = L'\0';
		info.PW = pw_from_stream;
		/// Read end

		// request sign in process
		auto resultData = Sign::SignManager::sInstance->SignInProcess(info);
		retResult = resultData.result;

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", info.ID.c_str(), info.PW.c_str());
		wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


		// get new send stream
		outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream (only data part)
		// sendpacket is composed with (size + id + data)

		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol
		outpStream->Write(&protocol, sizeof(EProtocol));
		// write result
		outpStream->Write(&resultData.result, sizeof(resultData.result));
		// write result msg
		int msg_length = static_cast<int>(wcslen(resultData.msg) * sizeof(wchar_t));	// 2bytes per one wchar
		outpStream->Write(&msg_length, sizeof(int));
		outpStream->Write(resultData.msg, msg_length);
		/// write end

#ifdef __DEBUG
		printf("SignState write to stream : %llubytes\n",
			sizeof(EProtocol) + sizeof(resultData.result) + msg_length);
#endif // __DEBUG		
	}
	break;

	case SignState::EProtocol::SignOut:
	{
		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfo info;
		info.Flush();
		// get id from stream
		wchar_t id_from_stream[Sign::SignInfo::MAX_IDSIZE];
		int id_length = 0;
		inpStream->Read(&id_length, sizeof(int));
		inpStream->Read(
			id_from_stream,
			id_length);
		// after last word must be \0
		// unicode is 2bytes per word
		id_from_stream[id_length / 2] = L'\0';
		info.ID = id_from_stream;
		// get pw from stream
		wchar_t pw_from_stream[Sign::SignInfo::MAX_PWSIZE];
		int pw_length = 0;
		inpStream->Read(&pw_length, sizeof(int));
		inpStream->Read(
			pw_from_stream,
			pw_length);
		// after last word must be \0
		// unicode is 2bytes per word
		pw_from_stream[pw_length / 2] = L'\0';
		info.PW = pw_from_stream;
		/// Read end

		// request sign in process
		auto resultData = Sign::SignManager::sInstance->SignInProcess(info);
		retResult = resultData.result;

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", info.ID.c_str(), info.PW.c_str());
		wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


		// get new send stream
		outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream (only data part)
		// sendpacket is composed with (size + id + data)

		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol
		outpStream->Write(&protocol, sizeof(EProtocol));
		// write result
		outpStream->Write(&resultData.result, sizeof(resultData.result));
		// write result msg
		int msg_length = static_cast<int>(wcslen(resultData.msg) * sizeof(wchar_t));	// 2bytes per one wchar
		outpStream->Write(&msg_length, sizeof(int));
		outpStream->Write(resultData.msg, msg_length);
		/// write end

#ifdef __DEBUG
		printf("SignState write to stream : %llubytes\n",
			sizeof(EProtocol) + sizeof(resultData.result) + msg_length);
#endif // __DEBUG	
	}
	break;

	case SignState::EProtocol::SignUp:
	{
		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfo info;
		info.Flush();
		// get id from stream
		wchar_t id_from_stream[Sign::SignInfo::MAX_IDSIZE];
		int id_length = 0;
		inpStream->Read(&id_length, sizeof(int));
		inpStream->Read(
			id_from_stream,
			id_length);
		// after last word must be \0
		// unicode is 2bytes per word
		id_from_stream[id_length / 2] = L'\0';
		info.ID = id_from_stream;
		// get pw from stream
		wchar_t pw_from_stream[Sign::SignInfo::MAX_PWSIZE];
		int pw_length = 0;
		inpStream->Read(&pw_length, sizeof(int));
		inpStream->Read(
			pw_from_stream,
			pw_length);
		// after last word must be \0
		// unicode is 2bytes per word
		pw_from_stream[pw_length / 2] = L'\0';
		info.PW = pw_from_stream;
		/// Read end

		// request sign in process
		auto resultData = Sign::SignManager::sInstance->SignInProcess(info);
		retResult = resultData.result;

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", info.ID.c_str(), info.PW.c_str());
		wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


		// get new send stream
		outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream (only data part)
		// sendpacket is composed with (size + id + data)

		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol
		outpStream->Write(&protocol, sizeof(EProtocol));
		// write result
		outpStream->Write(&resultData.result, sizeof(resultData.result));
		// write result msg
		int msg_length = static_cast<int>(wcslen(resultData.msg) * sizeof(wchar_t));	// 2bytes per one wchar
		outpStream->Write(&msg_length, sizeof(int));
		outpStream->Write(resultData.msg, msg_length);
		/// write end

#ifdef __DEBUG
		printf("SignState write to stream : %llubytes\n",
			sizeof(EProtocol) + sizeof(resultData.result) + msg_length);
#endif // __DEBUG		
	}
	break;

	case SignState::EProtocol::DeleteAccount:
	{
		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfo info;
		// get id from stream
		wchar_t id_from_stream[Sign::SignInfo::MAX_IDSIZE];
		int id_length = 0;
		inpStream->Read(&id_length, sizeof(int));
		inpStream->Read(
			id_from_stream,
			id_length);
		// after last word must be \0
		// unicode is 2bytes per word
		id_from_stream[id_length / 2] = L'\0';
		info.ID = id_from_stream;
		// get pw from stream
		wchar_t pw_from_stream[Sign::SignInfo::MAX_PWSIZE];
		int pw_length = 0;
		inpStream->Read(&pw_length, sizeof(int));
		inpStream->Read(
			pw_from_stream,
			pw_length);
		// after last word must be \0
		// unicode is 2bytes per word
		pw_from_stream[pw_length / 2] = L'\0';
		info.PW = pw_from_stream;
		/// Read end

		// request sign in process
		auto resultData = Sign::SignManager::sInstance->SignInProcess(info);
		retResult = resultData.result;

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", info.ID.c_str(), info.PW.c_str());
		wprintf(L" msg : %ws\n", resultData.msg);
#endif // __DEBUG


		// get new send stream
		outpStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream (only data part)
		// sendpacket is composed with (size + id + data)

		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol
		outpStream->Write(&protocol, sizeof(EProtocol));
		// write result
		outpStream->Write(&resultData.result, sizeof(resultData.result));
		// write result msg
		int msg_length = static_cast<int>(wcslen(resultData.msg) * sizeof(wchar_t));	// 2bytes per one wchar
		outpStream->Write(&msg_length, sizeof(int));
		outpStream->Write(resultData.msg, msg_length);
		/// write end

#ifdef __DEBUG
		printf("SignState write to stream : %llubytes\n",
			sizeof(EProtocol) + sizeof(resultData.result) + msg_length);
#endif // __DEBUG	
	}
	break;

	default:
		break;
	}


	// apply result to session
	switch (retResult)
	{
	case Sign::SignManager::EResult::Success_SingIn:
	{
		auto owner = m_ownerPtr.lock();
		owner->m_isSigned = true;

		owner->ChangeState(owner->m_characterSelect_state);
	}
	break;
	case Sign::SignManager::EResult::Success_SignOut:
	case Sign::SignManager::EResult::Success_DeleteAccount:
	{
		auto owner = m_ownerPtr.lock();
		owner->m_isSigned = false;
	}
	break;
	}


}

void SignState::OnSendCompleted()
{

}

IOCPSession::ClientStatePtr SignState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<SignState> retState;
	retState.reset(new SignState(pSession));

	return retState;
}
