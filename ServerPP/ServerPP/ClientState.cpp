#include "base.h"


IOCPSession::Signal LobbyState::OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr&)
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

IOCPSession::Signal IdleState::OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr&)
{
	return 0;
}

void IdleState::OnSendCompleted()
{

}
