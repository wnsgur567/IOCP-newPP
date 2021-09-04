#include "base.h"

//#define TESTING

IOCPSession::Signal SignState::OnRecvCompleted(InputMemoryStreamPtr inpStream, __out OutputMemoryStreamPtr)
{

	// ...
#ifdef TESTING

	printf("�׽�Ʈ ����\n");

	char msg[512];

	int msg_length = 0;
	inpStream->Read(&msg_length, sizeof(msg_length));
	inpStream->Read(&msg, static_cast<size_t>(msg_length));
	msg[msg_length + 1] = NULL;


	printf("\n���۵� �޽��� ���� : %d\n", msg_length);
	printf("���۵� �޼��� : %s", msg);


	auto pSendStream = PacketManager::sInstance->GetSendStreamFromPool();

	pSendStream->Write(&msg_length, sizeof(msg_length));
	pSendStream->Write(msg, msg_length);

	printf("\n�״�� �ٽ� ����...\n");
	
	return pSendStream;
#endif

	return 0;
}

void SignState::OnSendCompleted(IOCPSessionPtr inpSession)
{
	// state�� ����Ǿ��� ��� ������ ����...
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
