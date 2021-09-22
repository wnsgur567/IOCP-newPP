#include "IOCPNet_RootHeader.h"

void DungeonState::OnRecvCompleted(NetBase::InputMemoryStreamPtr)
{
}

void DungeonState::OnSendCompleted()
{
}

void DungeonState::OnInitilzed()
{
}

void DungeonState::OnChangedToThis()
{
}

IOCPSession::ClientStatePtr DungeonState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<DungeonState> retState;
	retState.reset(new DungeonState(pSession));

	return retState;
}
