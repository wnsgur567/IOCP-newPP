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
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;

	void GetProtocol(ProtocolSize_t, EProtocol&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);	
};
