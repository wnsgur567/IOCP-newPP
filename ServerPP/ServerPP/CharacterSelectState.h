#pragma once

class CharacterSelectState : public IOCPSession::ClientState
{
private:
	using EProtocol = CharacterSelectManager::EProtocol;
	using EResult = CharacterSelectManager::EResult;
protected:
	CharacterSelectState(IOCPSessionPtr inOwnerSession) : ClientState(inOwnerSession) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;

	void GetProtocol(ProtocolSize_t, EProtocol&);
	void HandleCharacterSelectPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	void HandleSignOutPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);	
};
