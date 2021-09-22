#pragma once

class DungeonState : public IOCPSession::ClientState
{
private:
	
protected:
	DungeonState(IOCPSessionPtr inOwnerSession)
		: ClientState(inOwnerSession) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr) override;
	virtual void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;


	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};

