#include "base.h"


OutputMemoryStreamPtr SignState::OnRecvCompleted(InputMemoryStreamPtr)
{
	return OutputMemoryStreamPtr();
}

void SignState::OnSendCompleted()
{
}

OutputMemoryStreamPtr LobbyState::OnRecvCompleted(InputMemoryStreamPtr)
{
	return OutputMemoryStreamPtr();
}

void LobbyState::OnSendCompleted()
{
}

OutputMemoryStreamPtr IdleState::OnRecvCompleted(InputMemoryStreamPtr)
{
	return OutputMemoryStreamPtr();
}

void IdleState::OnSendCompleted()
{
	
}
