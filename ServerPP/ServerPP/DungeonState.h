#pragma once

class DungeonState : public IOCPSession::ClientState
{
private:
	
protected:
	DungeonState(IOCPSessionPtr inOwnerSession)
		: ClientState(inOwnerSession) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis(NetBase::OutputMemoryStreamPtr&) override;


	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};

