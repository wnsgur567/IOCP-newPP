#pragma once

class CharacterSelectState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : ProtocolSize_t
	{

	};
protected:
	CharacterSelectState(IOCPSessionPtr inOwnerSession) : ClientState(inOwnerSession) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	void OnSendCompleted() override;

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};
