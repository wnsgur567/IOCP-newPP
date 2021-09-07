#include "base.h"


IOCPSession::Signal LobbyState::OnRecvCompleted(InputMemoryStreamPtr, OutputMemoryStreamPtr&)
{
	return 0;
}

void LobbyState::OnSendCompleted()
{
}

IOCPSession::ClientStatePtr LobbyState::Create()
{
	return IOCPSession::ClientStatePtr();
}

IOCPSession::Signal IdleState::OnRecvCompleted(InputMemoryStreamPtr, OutputMemoryStreamPtr&)
{
	return 0;
}

void IdleState::OnSendCompleted()
{

}
