#include "base.h"

//#define TESTING

IOCPSession::Signal SignState::OnRecvCompleted(InputMemoryStreamPtr inpStream, __out OutputMemoryStreamPtr)
{

	// ...
#ifdef TESTING

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

	return 0;
}

void SignState::OnSendCompleted(IOCPSessionPtr inpSession)
{
	// state가 변경되야할 경우 다음과 같이...
	ChangeState(inpSession, EState::Lobby, LobbyState::Create);
}

IOCPSession::ClientStatePtr SignState::Create()
{
	std::shared_ptr<SignState> retState = std::make_shared<SignState>();

	return retState;
}

IOCPSession::Signal LobbyState::OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr)
{
	return 0;
}

void LobbyState::OnSendCompleted(IOCPSessionPtr)
{
}

IOCPSession::ClientStatePtr LobbyState::Create()
{
	return IOCPSession::ClientStatePtr();
}

IOCPSession::Signal IdleState::OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr)
{
	return 0;
}

void IdleState::OnSendCompleted(IOCPSessionPtr)
{

}
