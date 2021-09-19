#include "IOCPNet_RootHeader.h"

void DungeonState::OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{
}

void DungeonState::OnSendCompleted()
{
}

void DungeonState::OnInitilzed()
{
}

void DungeonState::OnChangedToThis(NetBase::OutputMemoryStreamPtr&)
{
}

IOCPSession::ClientStatePtr DungeonState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<DungeonState> retState;
	retState.reset(new DungeonState(pSession));

	return retState;
}
