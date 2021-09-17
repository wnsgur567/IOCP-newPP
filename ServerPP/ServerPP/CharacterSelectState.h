#pragma once

class CharacterSelectState : public IOCPSession::ClientState
{
private:
	using EProtocol = CharacterSelect::CharacterSelectManager::EProtocol;
	using EResult = CharacterSelect::CharacterSelectManager::EResult;
	EResult m_current_result;
protected:
	CharacterSelectState(IOCPSessionPtr inOwnerSession) 
		: ClientState(inOwnerSession), m_current_result() {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis(NetBase::OutputMemoryStreamPtr&) override;

	void GetProtocol(ProtocolSize_t, EProtocol&);
	void HandleCharacterSelectPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	void HandleSignOutPacket(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);	
};
